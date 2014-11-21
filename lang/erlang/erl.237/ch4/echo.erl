-module(echo).
-export([start/0,print/1,stop/0,loop/0]).
%%   impl by func
%% start()->{ok}.
%% print(Term)->io:format("~p~n",[Term]),{ok}.
%% stop()->{ok}.

start()-> register(myecho,spawn(echo,loop,[])).

print(Term)-> myecho ! {print,Term},{ok}.

stop()->myecho ! {stop}.

loop()->
	receive
		{stop}->{ok};
		{print,Term}->io:format("~p~n",[Term]),loop();
		_Other->{error,unkown_msg},loop()
	end.
