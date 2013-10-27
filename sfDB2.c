#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>

#include <openssl/sha.h>

#include "sfDB2.h"

table *sfDB2_mkTable (uint8_t *name, uint16_t columns, uint8_t **column_name) {
	int32_t x;

	table *newTable = malloc (sizeof (table));
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

int main (void) {
	printf ("sizeof:\n\tstep:\t%lu\n\trow:\t%lu\n\tcoordinate:\t%lu\n\ttable:\t%lu\n",
		sizeof (step), sizeof (row), sizeof (coordinate), sizeof (table));
	return 0;
}
