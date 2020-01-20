#include "all.h"

#define MAX_TOKEN_SIZE 1024

struct Lexer {
	FILE *f;        /* Input source. */
	char *text;     /* Malloc'd buffer of lexeme text. */
	char *cur;      /* Pointer to last char in 'text'. */
	int   undid;    /* Set to 1 after lexundo, 0 otherwise, */
	int   tok;      /* Last token lexed (used for lexundo). */
};

static void
reset(Lexer *x)
{
	assert(x);
	x->cur = x->text;
	x->text[0] = '\0';
}

static int
next(Lexer *x)
{
	int c;
	assert(x);
	if (x->cur - x->text >= MAX_TOKEN_SIZE - 1)
		assert(!"token too long");  /* eh. */
	c = fgetc(x->f);
	*x->cur++ = c;
	*x->cur = '\0';
	return c;
}

static void
undo(Lexer *x, int c)
{
	assert(x);
	assert(x->cur > x->text);
	ungetc(c, x->f);
	*--x->cur = '\0';
}

Lexer*
lexinit(FILE *f)
{
	Lexer *x;
	assert(f);
	x = xmalloc(sizeof(Lexer));
	x->text = xmalloc(MAX_TOKEN_SIZE);
	x->undid = 0;
	x->f = f;
	reset(x);
	return x;
}

static int
dolex(Lexer *x)
{
	int c;
Again:
	reset(x);
	c = next(x);
	if (isspace(c))
		goto Again;
	switch (c) {
	case EOF:
		return TOK_EOF;
	case '\\':
		return TOK_BACKSLASH;
	case '.':
		return TOK_DOT;
	case '(':
		return TOK_LPAREN;
	case ')':
		return TOK_RPAREN;
	case '#':
		/* comments: #.*\n */
		while (c != '\n' && c != EOF)
			c = next(x);
		goto Again;
	}
	/* identifiers: [a-zA-Z_][a-zA-Z0-9'_]* */
	if (isalpha(c) || c == '_') {
		while (isalpha(c) || isdigit(c) || strchr("'_", c))
			c = next(x);
		undo(x, c);
		return TOK_IDENT;
	}
	return TOK_UNKNOWN;
}

int
lex(Lexer *x)
{
	assert(x);
	if (x->undid) {
		x->undid = 0;
		return x->tok;
	}
	x->tok = dolex(x);
	return x->tok;
}

void
lexundo(Lexer *x)
{
	assert(x);
	assert(x->undid == 0);
	x->undid = 1;
}

char*
lextext(Lexer *x)
{
	return x->text;
}

