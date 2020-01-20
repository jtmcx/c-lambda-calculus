TARG    = lambda
CFLAGS += -std=c89 -Wall -Werror -pedantic
CFLAGS += -D_DEFAULT_SOURCE -D_BSD_SOURCE
OFILES  =\
src/dump.o \
src/lex.o \
src/main.o \
src/parse.o \
src/reduce.o \
src/shift.o \
src/subst.o \
src/term.o \
src/util.o \


$(TARG) : $(OFILES)
	$(CC) -o $@ $(LDFLAGS) $(OFILES)

$(OFILES) : src/all.h

clean:
	rm -f $(TARG) $(OFILES)
