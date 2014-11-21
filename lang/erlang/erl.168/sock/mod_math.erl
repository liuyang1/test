-module(mod_math).
-export([run/3]).

run(MM,Argc,Args)->
	io:format("mathMod:run starting~n"
		"Argc = ~p Args = ~p~n",[Argc,Args]),
	loop(MM).

loop(MM)->
	receive
		{chan,MM,{factorial,N}}->
			MM ! {send, fac(N)},
			loop(MM);
		{chan,MM,{fibnacci,N}}->
			MM ! {send,fib(N)},
			loop(MM);
		{chan_closed,MM}->
			io:format("mathMod stopping~n"),
			exit(normal)
	end.

fac(0)->1;
fac(N)->N*fac(N-1).

fib(1)->1;
fib(2)->2;
fib(N)->fib(N-1)+fib(N-2).
