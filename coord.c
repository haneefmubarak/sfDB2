#include "sfDB2.h"

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

coordinate *sfDB2_mkCoord (const table *curTable, const uint16_t column,
			const uint8_t *matchtext, const uint64_t mtlength) {
	// Get the hash for the matchtext
	uint8_t *hashID = malloc (SHA_DIGEST_LENGTH);
	SHA1 (matchtext, mtlength, hashID);

	// Get to the end of the step tree
	navStepReturn nsr = sfDB2_navStep (curTable, column, hashID);

	// If something's already there, just pass that shit back
	if ((nsr.endstep->type[hashID[nsr.depth]] == 2) &&
	   (memcmp ((((coordinate *)nsr.endstep->next[hashID[nsr.depth]])->id), hashID,\
	   (SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES)) == 0)) {
		return nsr.endstep->next[hashID[nsr.depth]];
	}

	// Otherwise, setup the table
	coordinate *newCoord = calloc (1, sizeof (coordinate));
	newCoord->matchtext = memcpy (malloc (mtlength), matchtext, mtlength);
	memcpy (newCoord->id, hashID, SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES);
	newCoord->mtlength = mtlength;
	// Looks like it's all good in the hood
	newCoord->status = 0;

	// Cleanup
	free (hashID);

	return newCoord;
}
