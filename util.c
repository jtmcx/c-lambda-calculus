#include "all.h"

void*
xmalloc(size_t sz)
{
	void *p;
	p = malloc(sz);
	if (p == NULL)
		assert(!"out of memory");
	return p;
}

char*
xstrdup(char *s)
{
	char *p;
	p = strdup(s);
	if (p == NULL)
		assert(!"out of memory");
	return p;
}
