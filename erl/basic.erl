-module(basic).
-export([mirror/1]).
-export([num/1]).
-export([fib/1]).
-export([fact/1]).
-export([str2c/1]).
-export([cnt10/1]).

mirror(Anything)	-> Anything.

num(one)	->	1;
num(two)	->	2;
num(tri)	->	3.

fib(0)	->	1;
fib(1)	->	1;
fib(N)	->	fib(N-1)+fib(N-2).

fact(1)	->	1;
fact(N)	->	N*fact(N-1).

str2c(Str)	->	Str.

cnt10(N)	->
	if
		N>10	->	cnt10(N-1);
		N<10	->	cnt10(N+1);
		true	->	10
	end.
