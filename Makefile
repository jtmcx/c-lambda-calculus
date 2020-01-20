TARG    = lambda
CFLAGS += -std=c89 -Wall -Werror -pedantic
CFLAGS += -D_DEFAULT_SOURCE -D_BSD_SOURCE
HFILES  = all.h
OFILES  =\
dump.o \
lex.o \
main.o \
parse.o \
reduce.o \
shift.o \
subst.o \
term.o \
util.o \


$(TARG) : $(OFILES)
	$(CC) -o $@ $(LDFLAGS) $(OFILES)

$(OFILES) : $(HFILES)

clean:
	rm -f $(TARG) $(OFILES)
