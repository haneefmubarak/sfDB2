#define SFDB2_SHARD_BYTES 0

typedef uint8_t * IDMatch;

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
        IDMatch *match;
        uint8_t id[SHA_DIGEST_LENGTH - SFDB2_SHARD_BYTES];
        uint32_t matches;
        uint8_t status;
} coordinate;

typedef struct {
        uint8_t *name, **column_name;
        row **rows;
        step head;
        uint16_t columns;
} table;
