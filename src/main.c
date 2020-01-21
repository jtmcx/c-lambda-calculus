#include "all.h"

int
main(int argc, char *argv[])
{
	Term *t;
	if ((t = parse(stdin)) == NULL)
		return 1;
	t = reduce(t);
	dump(stdout, t);
	fprintf(stdout, "\n");
	return 0;
}
