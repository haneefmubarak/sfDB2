#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>

#include <openssl/sha.h>

#ifndef SFDB2_SHARD_BYTES
#define SFDB2_SHARD_BYTES 0
#endif
#ifndef SFDB2_PRAGMA_PACK
#define SFDB2_PRAGMA_PACK 1
#endif

//extern uint8_t invalid_sentinel;
//#define INVALID_POINTER ((void *)&invalid_sentinel)

#if SFDB2_PRAGMA_PACK > 0
#pragma pack(push,1)
#endif

extern uint8_t *error_sentinel[16];

typedef struct {
	void *next[256];
	uint8_t type[256];
	uint8_t status;
} step;

typedef struct {
	uint8_t **content;
	uint64_t *length;
	uint64_t id;
} row;

typedef struct {
	uint8_t *match;
	uint8_t id[SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES];
	uint32_t matches;
	uint8_t status;
} coordinate;

typedef struct {
	uint8_t *name, **column_name;
	uint8_t config[32];
	step **head;
	uint64_t rowcount;
	uint16_t columns;
} table;

#if SFDB2_PRAGMA_PACK > 0
#pragma pack(pop)
#endif

table	*sfDB2_mkTable	(const uint8_t *name, const uint16_t columns,
				const uint8_t **column_name, uint8_t config[32]);
step	*sfDB2_mkStep	(const table *curTable, const uint16_t column,
				const uint8_t *id, const uint8_t id_byte);
step	*sfDB2_navStep	(const table *curTable, const uint16_t column,
				const uint8_t *id);
