#include "all.h"

static Term* substvar(Term *t, int j, Term *s);
static Term* substlam(Term *t, int j, Term *s);
static Term* substapp(Term *t, int j, Term *s);

Term*
subst(Term *t, int j, Term *s)
{
	assert(t);
	assert(s);
	switch (t->tag) {
	case TERM_VAR:
		return substvar(t, j, s);
	case TERM_LAM:
		return substlam(t, j, s);
	case TERM_APP:
		return substapp(t, j, s);
	}
	assert(!"unreachable");
	return NULL;
}

static Term*
substvar(Term *t, int j, Term *s)
{
	assert(t);
	assert(t->tag == TERM_VAR);
	if (t->u.var == j)
		return s;
	return t;
}

static Term*
substlam(Term *t, int j, Term *s)
{
	assert(t);
	assert(t->tag == TERM_LAM);
	return mklam(subst(t->u.lam, j+1, shiftup(s)));
}

static Term*
substapp(Term *t, int j, Term *s)
{
	Term *lhs, *rhs;
	assert(t);
	assert(t->tag == TERM_APP);
	lhs = subst(t->u.app.lhs, j, s);
	rhs = subst(t->u.app.rhs, j, s);
	return mkapp(lhs, rhs);
}
