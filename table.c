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
	newTable->head.status = 1;
	newTable->rowcount = 0;

	// malloc the columns
	newTable->column_name = malloc (columns * (sizeof (uint8_t *)));
	assert (newTable->column_name);
	for (x = 0; x < columns; x++) {
		newTable->column_name[x] = (uint8_t *) strdup ((char *) column_name[x]);
		assert (newTable->column_name[x]);
	}

	return newTable;
}