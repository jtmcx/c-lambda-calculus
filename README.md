# Lambda Calculus

An implementation of the untyped [lambda calculus][1] in C. The resulting
program reads lambda calculus expressions from stdin, and performs full
normal-order reduction on the expression.

This was a little weekend project, so it isn't very sophisticated. There
is no garbage collection, and errors messages are obscure.

See the [examples](examples) folder for sample inputs.

## Building

	$ make

## Running

The following example applies the successor function to zero:

	$ ./lambda <<< "(\n.\s.\z. s (n s z)) (\s.\z. z)"
	\a. \b. a b

[1]: https://en.wikipedia.org/wiki/Lambda_calculus
