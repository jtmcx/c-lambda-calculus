#include "all.h"

static void
usage(char *argv0)
{
	fprintf(stderr, "usage: %s [file]\n", argv0);
	exit(1);
}

int
main(int argc, char *argv[])
{
	Term *t;
	FILE *file;
	char *filename;

	if (argc > 2)
		usage(argv[0]);

	if (argc == 2) {
		filename = argv[1];
		file = fopen(filename, "r");
		if (file == NULL) {
			perror("open");
			return 1;
		}
	} else {
		filename = "stdin";
		file = stdin;
	}

	t = parse(filename, file);
	if (t == NULL)
		return 1;

	t = reduce(t);
	dump(stdout, t);
	fprintf(stdout, "\n");
	return 0;
}
