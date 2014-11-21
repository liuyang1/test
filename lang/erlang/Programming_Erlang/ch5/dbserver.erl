-module(dbserver).
-export([start/0,stop/0,write/2,delete/1,read/1,match/1,loop/1]).
-import(db).

start()			-> register(dbserver,spawn(dbserver,loop,[[]])).

stop()			-> dbserver ! {self(),stop}.
write(Key,Value)-> dbserver ! {self(),write,{Key,Value}}.
delete(Key)		-> dbserver ! {self(),delete,{Key}}.
read(Key)		-> dbserver ! {self(),read,{Key}}.
match(Value)	-> dbserver ! {self(),match,{Value}}.

loop(Db)->
	receive
		{Pid,stop}				-> Pid ! {ok};
		{Pid,write,{Key,Value}} -> NewDb=db:write(Key,Value,Db),
			Pid ! {NewDb},loop(NewDb);
		{Pid,delete,{Key}}		-> NewDb=db:delete(Key,Db),
			Pid ! {NewDb},loop(NewDb);
		{Pid,read,{Key}}		-> Ret=db:read(Key,Db),
			Pid ! {Ret},loop(Db);
		{Pid,match,{Value}		-> Ret=db:match(Value,Db),
			Pid ! {Ret},loop(Db)
	end.
