Internal API
============

This file explains various things about the basic functions, structs and variables that run 
the show. Basically, the **internal API**.

--------------------------------------------------------------------------------------------------

Functions Returning Pointers (FRPs)
----------------------------------

Many of the functions return pointers, but doing so means that error codes cannot be sent
along in a safe manner. Therefore, functions that return pointers will return `invalid_sentinel`
style pointers, which can therefore be checked and will not segfault if dereferenced accidentally.
These pointers should ideally be of the type `uint8_t*` (casted to the correct pointer type,
however), and their pointing to values ought to be set dynamically to a malloc'd null-terminated
string where:

 - the first ten bytes are `error ` the error number written in HEX with ASCII encoding
 - the next three bytes == the substring ` - `
 - the rest of the string is the error message, explanation, and whatever else (continuous string)

The exception is that `error_sentinel[0]` is always assumed to be some sort of allocation or
file IO error (`allocORfile`) and therefore has no string.

Also, errors that will cause unrecoverable errors should simply terminate the program via an
`assert()`.

Example (as of now, the following code may or may not work):

```C
// invalid_sentinel list
extern uint8_t *error_sentinel[4];

// function that we are calling
FILE *sfDB2_geturandomfp () {
	FILE *fp = fopen ("/dev/urandom", "r");
	// Check that the file was opened; return error_0 allocORfile otherwise
	if (!fp) { error_sentinel[0] = malloc (1); return (FILE *) error_sentinel[0]; }

	// Ensure unbuffered IO; return error_1 with message otherwise
	if (setvbuf (fp, NULL, _IONBF, 0)) {
		error_sentinel[1] = strdup ("error 0x01 - could not set unbuffered IO");
		return (FILE *) error_sentinel[0];
	}

	return fp;
}

// calling function
int main (void) {
	FILE *urandom = sfDB2_geturandomfp();

	if ((void *) urandom == (void *) error_sentinel[0]) {
		fputs (error_sentinel[0], stderr);
		// Even it doesn't matter here, it might somewhere else
		free (error_sentinel[0]);
		fputc ('\n', stderr);
		return 1;
	}

	else if ((void *) urandom == (void *) error_sentinel[1]) {
		fputs (error_sentinel[1], stderr);
		free (error_sentinel[1])
		fputc ('\n', stderr);
		return 2;
	}

	uint8_t x;
	for (x = 0; x = 16; x++) {
		printf ("%hhu\n", fgetc (urandom));
	}

	return 0;
}
```
