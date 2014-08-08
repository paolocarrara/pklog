#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>

#define BUFFER_SZ 32

int main (int argc, char **argv)
{
	struct input_event key;
	char *buffer = malloc (BUFFER_SZ*sizeof(char));
	char *input = NULL;
	char *output = "output";
	int opt;
	int infp, outfp;

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

	/* opens input */
	infp = open (input, O_RDONLY);

	/* opens output */
	outfp = open (output, O_WRONLY | O_CREAT | O_APPEND);

	while (1) {
		read (infp, &key, sizeof(struct input_event));
		if (key.type == 1)
			printf ("Tecla pressionada\n");
	}


	return 0;
}
