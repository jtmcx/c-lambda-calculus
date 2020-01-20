#include "all.h"

Term*
mkvar(int var)
{
	Term *t;
	t = xmalloc(sizeof(Term));
	t->tag = TERM_VAR;
	t->u.var = var;
	return t;
}

Term*
mklam(Term *lam)
{
	Term *t;
	assert(lam);
	t = xmalloc(sizeof(Term));
	t->tag = TERM_LAM;
	t->u.lam = lam;
	return t;
}

Term*
mkapp(Term *lhs, Term *rhs)
{
	Term *t;
	assert(lhs);
	assert(rhs);
	t = xmalloc(sizeof(Term));
	t->tag = TERM_APP;
	t->u.app.lhs = lhs;
	t->u.app.rhs = rhs;
	return t;
}
