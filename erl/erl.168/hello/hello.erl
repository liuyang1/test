-module(hello).
-export([start/0]).

start()->
    io:format("hello, world~n").
