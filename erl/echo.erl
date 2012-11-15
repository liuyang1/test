-module(echo).
-author("liuy").
-export([startd/0,start/0,start/1,process/1]).
-define(defPort,2000).

startd()	->
	register(eserverp,spawn(?MODULE,start,[])).

start()		->
	start(?defPort).

start(Port)	->
	case gen_tcp:listen(Port,[binary,{packet,0},{active,false}]) of
		{ok,LSock}	-> 
			server_loop(LSock);
		{error,Reason}	->	
			exit(Reason)
	end.

server_loop(LSock)	->
	case gen_tcp:accept(LSock)	of 
		{ok,Sock}	->
			spawn(?MODULE,process,[Sock]),
			server_loop(LSock);
		{error,Reason}	->
			exit(Reason)
	end.

%% this is a comment
process(Sock)	->
	Req	=	do_recv(Sock),
	io:format("~p~n",[Req]),
	Resp=	"erlang:"++Req,
	do_send(Sock,Resp),
	process(Sock),
	gen_tcp:close(Sock).

do_send(Sock,Msg)	->
	case gen_tcp:send(Sock,Msg)	of
		ok	->	ok;
		{error,Reason}	->	exit(Reason)
	end.

do_recv(Sock)	->
	case gen_tcp:recv(Sock,0) of
		{ok,Bin}	->	binary_to_list(Bin);
		{error,closed}	->	exit(closed);
		{error,Reason}	->	exit(Reason)
	end.
