#pragma once

#define SFDB2_SHARD_BYTES 0

extern uint8_t invalid_sentinel;
#define INVALID_POINTER ((void *)&invalid_sentinel)

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
        step head, rowhead;
	uint64_t rowcount;
        uint16_t columns;
} table;

table	*sfDB2_mkTable	(const uint8_t *name, const uint16_t columns,
				const uint8_t **column_name);
step	*sfDB2_mkStep	(const table *curTable, const uint8_t *id,
				const uint8_t id_byte);
step	*sfDB2_navStep (const table *curTable, const uint8_t *id);
