#include "sfDB2.h"

table *sfDB2_mkTable (const uint8_t *name, const uint16_t columns,
			const uint8_t **column_name) {
	int32_t x;

	// Fill the table with zeros so we don't have to zero specific fields
	table *newTable = calloc (1, sizeof (table));
	assert (newTable);

	// Fill in the basic info
	newTable->name = (uint8_t *) strdup ((char *) name);
	assert (newTable->name);
	newTable->columns = columns;
	newTable->rowcount = 0;

	// malloc the columns
	newTable->column_name = malloc (columns * (sizeof (uint8_t *)));
	assert (newTable->column_name);
	for (x = 0; x < columns; x++) {
		newTable->column_name[x] = (uint8_t *) strdup ((char *) column_name[x]);
		assert (newTable->column_name[x]);
	}

	// malloc the column indices
	newTable->head = malloc (sizeof (step *) * (columns + 1)); // +1 is for the row index
	// calloc the row index
	newTable->head[0] = calloc (1, sizeof (step));
	newTable->head[0]->status = 1;
	// calloc the rest
	for (x = 1; x < (columns + 1); x++) {
		newTable->head[x] = calloc (1, sizeof (step));
		newTable->head[x]->status = 1;
	}

	return newTable;
}
