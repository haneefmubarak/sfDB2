#include "sfDB2.h"

step *sfDB2_mkStep (const table *curTable, const uint16_t column,
			const uint8_t *id, const uint8_t id_byte) {
	int16_t x, status;
	step *curStep;
	coordinate *oldCoord;

	if (!(id_byte < (SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES))) {
		error_sentinel[1] = (uint8_t *) strdup
			("error 0x01 - id_byte is larger than its maximum");
		return (step *) error_sentinel[1];
	}

	assert (curTable->head[column]->status);

	curStep = (curTable->head[column]);

	// Traverse the tree
	status = curStep->type[id[0]];
	for (x = 0; status; x++) {
		curStep = curStep->next[id[x]];
		status = ((x != id_byte) && (curStep->type[id[x]] == 1));
	}

	// If we're already here, just send the thing back
	if (x == id_byte) { return curStep; }

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
		newStep = sfDB2_mkStep (curTable, column, id, id_byte);
	}

	return newStep;
}

step *sfDB2_navStep (const table *curTable, const uint16_t column,
			const uint8_t *id) {
	uint16_t x, status;
	step *curStep = (curTable->head[column]);

	for (x = 0; status; x++) {
		curStep = curStep->next[id[x]];
		status = (curStep->type[id[x]] == 1);
	}

	// Ensure that we have either hit an empty * or row *, respectively
	assert ((curStep->type[id[x]] == 0) || (curStep->type[id[x]] == 2));

	return curStep;
}
