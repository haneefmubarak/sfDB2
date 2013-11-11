#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>

#include <openssl/sha.h>

// Sharding based on the first n-bytes of the SHA hash
#ifndef SFDB2_SHARD_BYTES
#define SFDB2_SHARD_BYTES 0
#endif
#ifndef SFDB2_PRAGMA_PACK
#define SFDB2_PRAGMA_PACK 1
#endif

// Old example left for posterity + idea of how the sentinel thing works

//extern uint8_t invalid_sentinel;
//#define INVALID_POINTER ((void *)&invalid_sentinel)

#if SFDB2_PRAGMA_PACK > 0
#pragma pack(push,1)
#endif

// The error sentinel used for storing errors and shit
extern uint8_t *error_sentinel[16];

// A single step down the hashtree, one byte at a time
typedef struct {
	void *next[256];	// either a coordinate or a step
	uint8_t type[256];	// whether next is null, a coordinate, or a step
	uint8_t status;		// whether this is the end of the line (0 = yes, 1 = no)
} step;

// An indexed entry with data from the columns
typedef struct {
	uint8_t **content;	// the content from each of the columns
	uint64_t *length;	// length of ^^^^^^^^^^^^^^^^^^^^^^^^^^
	uint64_t id;		// incremented id, used mainly for internal ops
} row;

// A unique entry in a columns, used for indexing the rows
typedef struct {
	row **match;		// pointers to the indexed matches
	uint8_t *matchtext;	// what exactly was matched
	uint8_t id[SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES]; // unique SHA1 hash for indexing
	uint64_t mtlength;	// length of the matchtext
	uint32_t matches;	// number of indexed matches
	uint8_t status;		// status of the coordinate
} coordinate;

// A table which holds massive quantities of all of the above in an organized fashion
typedef struct {
	uint8_t *name, **column_name; // name of the table + names of the columns
	uint8_t config[32];	// config arrar of bits
	step **head;		// start of hashtree for each column + the row index
	uint64_t rowcount;	// number of rows in this table (or at least this server's portion)
	uint16_t columns;	// number of columns in this table
} table;

typedef struct {
	step *endstep;		// the step to be returned
	uint8_t depth;		// how far down the tree ^ is
} navStepReturn;

#if SFDB2_PRAGMA_PACK > 0
#pragma pack(pop)
#endif

// Make a table and initialize it
table		*sfDB2_mkTable	(const uint8_t *name, const uint16_t columns,
				const uint8_t **column_name, uint8_t config[32]);

// Make a step if it doesn't already exist
step		*sfDB2_mkStep	(const table *curTable, const uint16_t column,
				const uint8_t *id, const uint8_t id_byte);

// Get to the step matching an id in the respective tree
navStepReturn	sfDB2_navStep	(const table *curTable, const uint16_t column,
				const uint8_t *id);

// If the coord doesn't exist, make it
coordinate	*sfDB2_mkCoord	(const table *curTable, const uint16_t column,
				const uint8_t *matchtext, const uint64_t mtlength);
