#include "all.h"

static Term* reducelam(Term *t);
static Term* reduceapp(Term *t);

Term*
reduce(Term *t)
{
	assert(t);
	switch (t->tag) {
	case TERM_VAR:
		return t;
	case TERM_LAM:
		return reducelam(t);
	case TERM_APP:
		return reduceapp(t);
	}
	assert(!"unreachable");
	return NULL;
}

static Term*
reducelam(Term *t)
{
	assert(t);
	assert(t->tag == TERM_LAM);
	return mklam(reduce(t->u.lam));
}

static Term*
reduceapp(Term *t)
{
	Term *lhs;

	assert(t);
	assert(t->tag == TERM_APP);

	/* Start by reducing terms on the left hand side. */
	lhs = reduce(t->u.app.lhs);

	if (lhs->tag == TERM_LAM) {
		/* Beta reduce the application. */
		t = shiftdown(subst(lhs->u.lam, 0, shiftup(t->u.app.rhs)));

		/* Further reduce the expression (this will recurse until
		 * we have a "head normal form" for this expression). */
		return reduce(t);
	}

	/* If get here, the left hand side is in head normal form. We
	 * then further reduce the right hand side of the expression. */
	return mkapp(lhs, reduce(t->u.app.rhs));
}
