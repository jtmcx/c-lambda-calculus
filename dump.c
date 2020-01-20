#include "all.h"

static void dodump(FILE *f, Term *t, int depth);
static void ident(FILE *f, int index);
static void parens(FILE *f, Term *t, int depth);
static void dumpvar(FILE *f, Term *t, int depth);
static void dumplam(FILE *f, Term *t, int depth);
static void dumpapp(FILE *f, Term *t, int depth);

void
dump(FILE *f, Term *t)
{
	dodump(f, t, 0);
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

static void
ident(FILE *f, int index)
{
	/* Print out a unique identifier for some De Bruijn index.
	Follows the pattern: a, b, c, ... aa, bb, cc, ... */

	static char letters[] = "abcdefghijklmnopqrstuvwxyz";

	int c = letters[index % 26];
	do {
		fprintf(f, "%c", c);
	} while ((index -= 26) >= 0);
}

static void
parens(FILE *f, Term *t, int depth)
{
	fprintf(f, "(");
	dodump(f, t, depth);
	fprintf(f, ")");
}

static void
dumpvar(FILE *f, Term *t, int depth)
{
	assert(t);
	assert(t->tag == TERM_VAR);
	ident(f, depth - t->u.var - 1);
}

static void
dumplam(FILE *f, Term *t, int depth)
{
	assert(t);
	assert(t->tag == TERM_LAM);
	fprintf(f, "\\");
	ident(f, depth);
	fprintf(f, ". ");
	dodump(f, t->u.lam, depth + 1);
}

static void
dumpapp(FILE *f, Term *t, int depth)
{
	assert(t);
	assert(t->tag == TERM_APP);

	/* Lambda's need to be parenthesized if they are on
	the left hand side of an application. */
	if (t->u.app.lhs->tag == TERM_LAM) {
		parens(f, t->u.app.lhs, depth);
	} else {
		dodump(f, t->u.app.lhs, depth);
	}

	fprintf(f, " ");

	/* Application is left associative, so if there is an
	application on the right side, we need to parenthesize.
	Lambda's are also easier to read if parenthesized. */
	if (t->u.app.rhs->tag != TERM_VAR) {
		parens(f, t->u.app.rhs, depth);
	} else {
		dodump(f, t->u.app.rhs, depth);
	}
}
