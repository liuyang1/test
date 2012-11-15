-module(myring2).
-export([start/1,run/3,stop/1,loop/1]).

%% ch4 exer2
%% quite tough!
%% remember it!
%% 
start(N)-> PidList=start_proc(N),
	hd(lists:reverse(PidList)) ! {switch,hd(PidList)}, %% this is core
	register(head,hd(lists:reverse(PidList))).

run(M,N,Msg)->head ! {M*N,Msg}.

stop(N)->head ! {N,stop}.

start_proc(1)->Pid=spawn(myring2,loop,[tmp]),io:format("this:~p next:~p~n",[Pid,tmp]),[Pid];
start_proc(N)->NextPid=start_proc(N-1),
	Pid=spawn(myring2,loop,[hd(NextPid)]),io:format("this:~p next:~p~n",[Pid,hd(NextPid)]),[Pid|NextPid].

loop(NextPid)->
	receive
		{stop}->{stop,ok};
		{switch,NewNextPid}->loop(NewNextPid);%% to switch NextPid,core
		{1,Msg}->io:format("~p ~p ~p ~p~n",[self(),NextPid,1,Msg]),loop(NextPid);
		{TTL,Msg}->io:format("~p ~p ~p ~p~n",[self(),NextPid,TTL,Msg]),NextPid ! {TTL-1,Msg},loop(NextPid)
	end.
