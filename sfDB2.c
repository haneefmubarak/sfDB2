#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>

#include <openssl/sha.h>

#include "sfDB2.h"

int main (void) {
	printf ("sizeof:\n\tstep:\t%lu\n\trow:\t%lu\n\tcoordinate:\t%lu\n\ttable:\t%lu\n",
		sizeof (step), sizeof (row), sizeof (coordinate), sizeof (table));
	return 0;
}

table *sfDB2_mkTable (const uint8_t *name, const uint16_t columns,
			const uint8_t **column_name) {
	int32_t x;

	table *newTable = calloc (1, sizeof (table));
	assert (newTable);

	newTable->name = (uint8_t *) strdup ((char *) name);
	assert (newTable->name);
	newTable->columns = columns;
	newTable->head.status = 1;
	newTable->rowcount = 0;

	newTable->column_name = malloc (columns * (sizeof (uint8_t *)));
	assert (newTable->column_name);
	for (x = 0; x < columns; x++) {
		newTable->column_name[x] = (uint8_t *) strdup ((char *) column_name[x]);
		assert (newTable->column_name[x]);
	}

	return newTable;
}

step *sfDB2_mkStep (const table *curTable, const uint8_t *id,
			const uint8_t id_byte) {
	int16_t x, status;
	step *curStep;
	coordinate *oldCoord;

	//assert (id_byte < (SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES));
	assert (curTable->head.status);

	curStep = (step *) &(curTable->head);

	// Traverse the tree
	status = curStep->type[id[0]];
	for (x = 0; status; x++) {
		curStep = curStep->next[id[x]];
		status = (!(x == id_byte) && (curStep->type[id[x]] == 1));
	}

	// Ensure that we have either hit an empty * or row *, respectively
	assert ((curStep->type[id[x]] == 0) || (curStep->type[id[x]] == 2));

	// If there's already soemthing here, save it for later
	if (curStep->type[id[x]] == 2) {
		oldCoord = curStep->next[id[x]];
	}

	step *newStep = calloc (1, sizeof (step));
	assert (newStep);

	curStep->next[id[x]] = newStep;

	// If there was something here, put it back
	if (curStep->type[id[x]] == 2) {
		uint8_t nextID = oldCoord->id[x];
		((step *)(curStep->next[id[x]]))->next[nextID] = oldCoord;
	}

	curStep->type[id[x]] = 1;

	// Done, but if we aren't at the id_byte we want, get there
	if (x < id_byte) {
		newStep = sfDB2_mkStep (curTable, id, id_byte);
	}

	return newStep;
}

step *sfDB2_navStep (const table *curTable, const uint8_t *id) {
	uint16_t x, status;
	step *curStep = (step *) &(curTable->head);

	for (x = 0; status; x++) {
		curStep = curStep->next[id[x]];
		status = (curStep->type[id[x]] == 1);
	}

	// Ensure that we have either hit an empty * or row *, respectively
	assert ((curStep->type[id[x]] == 0) || (curStep->type[id[x]] == 2));

	return curStep;
}
