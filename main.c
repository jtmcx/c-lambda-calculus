#include "all.h"

int
main(int argc, char *argv[])
{
	Term *t = parse(stdin);
	if (t == NULL)
		return 1;

	dump(stdout, t);
	fprintf(stdout, "\n");
	dump(stdout, reduce(t));
	fprintf(stdout, "\n");
	return 0;
}
