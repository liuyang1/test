-module(out).
-export([test/0]).

test()->
	io:format("~B~n",[31]),
	io:format("~.16B~n",[31]).
