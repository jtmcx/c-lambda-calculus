#include <assert.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Lexer  Lexer;
typedef struct Term   Term;

struct Term {
	enum {
		TERM_VAR,
		TERM_LAM,
		TERM_APP
	} tag;
	union {
		int   var;
		Term *lam;
		struct {
			Term *lhs;
			Term *rhs;
		} app;
	} u;
};

enum {
	TOK_UNKNOWN   = -1,
	TOK_EOF       = 0,
	TOK_BACKSLASH,
	TOK_DOT,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_IDENT
};

/* util.c */
void*   xmalloc(size_t);
char*   xstrdup(char *);

/* term.c */
Term*   mkvar(int var);
Term*   mklam(Term *lam);
Term*   mkapp(Term *lhs, Term *rhs);

/* shift.c */
Term*   shift(Term *, int cutoff, int amt);
Term*   shiftdown(Term *);
Term*   shiftup(Term *);

/* subst.c */
Term*   subst(Term *, int j, Term *s);

/* reduce.c */
Term*   reduce(Term *);

/* lex.c */
Lexer*  lexinit(FILE *);
int     lex(Lexer *);
char*   lextext(Lexer *);
void    lexundo(Lexer *);
int     lexline(Lexer *);

/* parse.c */
Term*   parse(char *filename, FILE *);

/* dump.c */
void    dump(FILE *, Term *);
