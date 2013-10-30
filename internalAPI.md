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

The exception is that `error_sentinel_1` is always assumed to be some sort of allocation or
file IO error.

Example (as of now, the following code may or may not work):

```C
// invalid_sentinel list
extern uint8_t *error_sentinel_1, *error_sentinel_2, *error_sentinel_3;

// function that we are calling
FILE *sfDB2_geturandomfp () {
	FILE *fp = fopen ("/dev/urandom", "r");
	// Check that the file was opened; return error_1 allocORfile otherwise
	if (!fp) { return (FILE *) error_sentinel_1; }

	// Ensure unbuffered IO; return error_2 with message otherwise
	if (setvbuf (fp, NULL, _IONBF, 0)) {
		error_sentinel_2 = strdup ("error 0x02 - could not set unbuffered IO");
		return (FILE *) error_sentinel_2;
	}

	return fp;
}

// calling function
int main (void) {
	FILE *urandom = sfDB2_geturandomfp();

	if ((void *) urandom == (void *) error_sentinel_1) {
		fputs (error_sentinel_1, stderr);
		fputc ('\n', stderr);
		return 1;
	}

	else if ((void *) urandom == (void *) error_sentinel_2) {
		fputs (error_sentinel_2, stderr);
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
