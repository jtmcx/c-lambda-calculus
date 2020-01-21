#include "all.h"

typedef struct Env    Env;
typedef struct Parser Parser;

struct Parser {
	Lexer   *lex;      /* Lexer. */
	Env     *env;      /* Identifiers in scope. */
	char    *errmsg;   /* Message from call to 'error'. */
	jmp_buf  jmp;      /* Yell at me all you like. */
};

struct Env {
	char *ident;       /* Identifier at this De Bruijn index. */
	Env  *next;        /* Next identifier on the stack. */
};

static char* tokname(int tok);
static void  pushenv(Parser *p, char *ident);
static void  popenv(Parser *p);
static int   debruijn(Env *env, char *ident);
static void  error(Parser *p, char *fmt, ...);
static char* expect(Parser *p, int expected);
static int   peek(Parser *p);
static int   isstartofterm(int tok);
static Term* parsevar(Parser *p);
static Term* parselam(Parser *p);
static Term* parseoneterm(Parser *p);
static Term* parseterm(Parser *p);

static char*
tokname(int tok)
{
	switch (tok) {
	case TOK_EOF:
		return "eof";
	case TOK_BACKSLASH:
		return "'\\'";
	case TOK_DOT:
		return "'.'";
	case TOK_LPAREN:
		return "'('";
	case TOK_RPAREN:
		return "')'";
	case TOK_IDENT:
		return "identifier";
	}
	return "unknown";
}

static void
pushenv(Parser *p, char *ident)
{
	Env *e = xmalloc(sizeof(Env));
	e->next = p->env;
	e->ident = xstrdup(ident);
	p->env = e;
}

static void
popenv(Parser *p)
{
	Env *e = p->env;
	p->env = e->next;
	free(e->ident);
	free(e);
}

static int
debruijn(Env *env, char *ident)
{
	Env *e;
	int i = 0;
	for (e = env; e != NULL; e = e->next, i++) {
		if (strcmp(ident, e->ident) == 0)
			return i;
	}
	return -1;
}

static void
error(Parser *p, char *fmt, ...)
{
	char buf[256];  /* "enough" */
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buf, 256, fmt, vl);
	p->errmsg = xstrdup(buf);
	va_end(vl);
	longjmp(p->jmp, 1);
}

static char*
expect(Parser *p, int expected)
{
	int tok = lex(p->lex);
	if (tok != expected)
		error(p, "expected %s, but found %s instead",
				tokname(expected), tokname(tok));
	return lextext(p->lex);
}

static int
peek(Parser *p)
{
	int tok = lex(p->lex);
	lexundo(p->lex);
	return tok;
}

static int
isstartofterm(int tok)
{
	switch (tok) {
	case TOK_IDENT:
	case TOK_BACKSLASH:
	case TOK_LPAREN:
		return 1;
	}
	return 0;
}

static Term*
parsevar(Parser *p)
{
	int index;
	char *ident = expect(p, TOK_IDENT);
	if ((index = debruijn(p->env, ident)) < 0)
		error(p, "identifier '%s' is not in scope", ident);
	return mkvar(index);
}

static Term*
parselam(Parser *p)
{
	Term *t;
	expect(p, TOK_BACKSLASH);
	pushenv(p, expect(p, TOK_IDENT));
	expect(p, TOK_DOT);
	t = parseterm(p);
	popenv(p);
	return mklam(t);
}

static Term*
parseoneterm(Parser *p)
{
	Term *t;
	switch (peek(p)) {
	case TOK_IDENT:
		return parsevar(p);
	case TOK_BACKSLASH:
		return parselam(p);
	case TOK_LPAREN:
		expect(p, TOK_LPAREN);
		t = parseterm(p);
		expect(p, TOK_RPAREN);
		return t;
	}
	error(p, "expected a variable, a lambda expression, "
	         "or a parenthesized expression");
	return NULL;	/* unreachable */
}

static Term*
parseterm(Parser *p)
{
	Term *t = parseoneterm(p);
	while (isstartofterm(peek(p)))
		t = mkapp(t, parseoneterm(p));
	return t;
}

Term*
parse(char *filename, FILE *f)
{
	Term *t;
	Parser p;
	p.lex = lexinit(f);
	p.errmsg = NULL;
	p.env = NULL;

	if (setjmp(p.jmp)) {
		/* Wacky way of simulating exceptions. Recursive decent
		parsers tend to be fairly verobse if every possible error
		is checked up front. Instead, we call longjmp to return
		to this point if an error occurred. */

		fprintf(stderr, "%s:%d: parse error: %s\n",
				filename, lexline(p.lex), p.errmsg);
		return NULL;
	}

	t = parseterm(&p);
	expect(&p, TOK_EOF);
	return t;
}
