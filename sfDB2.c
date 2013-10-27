#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>

#include <openssl/sha.h>

#include "sfDB2.h"

int main (void) {
	printf ("sizeof:\n\tstep:\t%lu\n\trow:\t%lu\n\tcoordinate:\t%lu\n\ttable:\t%lu\n",
		sizeof (step), sizeof (row), sizeof (coordinate), sizeof (table));
	return 0;
}
