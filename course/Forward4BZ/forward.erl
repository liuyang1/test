-module(forward).
-export([start/0,client/1]).

start() ->
	spawn(fun() -> server(1234) end).

server(Port)->
	{ok,InSocket} = gen_udp:open(Port,[binary,{active,false}]),
	io:format("forward server opened socket:~p~n",[InSocket]),
	{ok,OutSocket}= gen_udp:open(Port+1,[binary,{active,false}]),
	io:format("forward server opened socket:~p~n",[OutSocket]),
	loop(InSocket,OutSocket).

loop(InSocket,OutSocket) ->
	inet:setopts(InSocket,[{active,once}]),
	inet:setopts(OutSocket,[{active,once}]),
	receive
		{udp,InSocket,Host,Port,Bin} ->
			io:format("recv:~p from (~p,~p)~n",[Bin,Host,Port]),
			% gen_udp:send(InSocket,Host,Port,Bin),
			gen_udp:send(OutSocket,Host,Port,Bin),
			loop(InSocket,OutSocket)
	end.

client(N) ->
	{ok,Socket} = gen_udp:open(0,[binary]),
	io:format("client open socket ~p~n",[Socket]),
	ok = gen_udp:send(Socket,"localhost",1234,N),
	Value = receive
		{udp,Socket,_,_,Bin} ->
			io:format("client rece: ~p~n",[Bin])
		after 2000 ->
			0
	end,
	gen_udp:close(Socket),
	Value.
