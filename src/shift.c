#include "all.h"

static Term* shiftvar(Term *t, int cutoff, int amt);
static Term* shiftlam(Term *t, int cutoff, int amt);
static Term* shiftapp(Term *t, int cutoff, int amt);

Term*
shift(Term *t, int cutoff, int amt)
{
	assert(t);
	switch (t->tag) {
	case TERM_VAR:
		return shiftvar(t, cutoff, amt);
	case TERM_LAM:
		return shiftlam(t, cutoff, amt);
	case TERM_APP:
		return shiftapp(t, cutoff, amt);
	}
	assert(!"unreachable");
	return NULL;
}

Term*
shiftup(Term *t)
{
	return shift(t, 0, 1);
}

Term*
shiftdown(Term *t)
{
	return shift(t, 0, -1);
}

static Term*
shiftvar(Term *t, int cutoff, int amt)
{
	assert(t);
	assert(t->tag == TERM_VAR);
	if (t->u.var < cutoff)
		return t;
	return mkvar(t->u.var + amt);
}

static Term*
shiftlam(Term *t, int cutoff, int amt)
{
	Term *body;
	assert(t);
	assert(t->tag == TERM_LAM);
	body = shift(t->u.lam, cutoff+1, amt);
	return mklam(body);
}

static Term*
shiftapp(Term *t, int cutoff, int amt)
{
	Term *lhs, *rhs;
	assert(t);
	assert(t->tag == TERM_APP);
	lhs = shift(t->u.app.lhs, cutoff, amt);
	rhs = shift(t->u.app.rhs, cutoff, amt);
	return mkapp(lhs, rhs);
}
