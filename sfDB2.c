#include "sfDB2.h"

uint8_t *error_sentinel[16];

int main (void) {
	printf ("sizeof:\n\tstep:\t%lu\n\trow:\t%lu\n\tcoordinate:\t%lu\n\ttable:\t%lu\n",
		sizeof (step), sizeof (row), sizeof (coordinate), sizeof (table));
	return 0;
}