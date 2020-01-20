#include "all.h"

static char letters[] = "abcdefghijklmnopqrstuvwxyz";

static void dodump(FILE *f, Term *t, int depth);

static void
dumpvar(FILE *f, Term *t, int depth)
{
	assert(t);
	assert(t->tag == TERM_VAR);
	assert(t->u.var < 26);       /* This is really dumb. */
	fprintf(f, "%c", letters[depth - t->u.var - 1]);
}

static void
dumplam(FILE *f, Term *t, int depth)
{
	assert(t);
	assert(t->tag == TERM_LAM);
	assert(depth < 26);       /* This is also really dumb. */
	fprintf(f, "\\%c. ", letters[depth]);
	dodump(f, t->u.lam, depth+1);
}

static void
dumpapp(FILE *f, Term *t, int depth)
{
	assert(t);
	assert(t->tag == TERM_APP);
	if (t->u.app.lhs->tag != TERM_VAR) {
		fprintf(f, "(");
		dodump(f, t->u.app.lhs, depth);
		fprintf(f, ")");
	} else {
		dodump(f, t->u.app.lhs, depth);
	}
	fprintf(f, " ");
	if (t->u.app.rhs->tag == TERM_APP) {
		fprintf(f, "(");
		dodump(f, t->u.app.rhs, depth);
		fprintf(f, ")");
	} else {
		dodump(f, t->u.app.rhs, depth);
	}
}

static void
dodump(FILE *f, Term *t, int depth)
{
	assert(t);
	switch (t->tag) {
	case TERM_VAR:
		dumpvar(f, t, depth);
		break;
	case TERM_LAM:
		dumplam(f, t, depth);
		break;
	case TERM_APP:
		dumpapp(f, t, depth);
		break;
	}
}

void
dump(FILE *f, Term *t)
{
	dodump(f, t, 0);
}
