-module(freq).
-export([start/0,stop/0,alloc/0,dealloc/1,dump/0,match/0,count/0,loop/1]).

start()			->register(freqserver,spawn(freq,loop,[{[21,22,23],[]}])).

stop()			-> call(stop).
alloc()			-> call(alloc).
dealloc(F)		-> call({dealloc,F}).
match()			-> call(match).
count()			-> length(call(match)).
dump()			-> call(dump).

call(Msg) ->
	freqserver ! {request,self(),Msg},
	receive
		{reply,Reply} -> Reply
	end.

loop(Freq)->
	receive
		{request,Pid,alloc}->
			{NewFreq,Reply} = alloc(Freq,Pid),
			reply(Pid,Reply),loop(NewFreq);
		{request,Pid,{dealloc,F}}->
			NewFreq = dealloc(Freq,F,Pid),
			reply(Pid,ok),loop(NewFreq);
		{request,Pid,stop}->
			if
				([]==element(2,Freq)) -> reply(Pid,ok);
				true->reply(Pid,{error,"unrelease allocted freq"}),loop(Freq)
			end;
		{request,Pid,dump}->
			reply(Pid,Freq),loop(Freq);
		{request,Pid,match}->
			reply(Pid,match(Pid,getAllocated(Freq))),loop(Freq)
	end.

reply(Pid,Reply)-> Pid ! {reply,Reply}.

alloc({[],Allocated},_Pid)			-> {{[],Allocated},{error,no_freq}};
alloc({[Freq|Free],Allocated},Pid)	-> 
	Cnt=length(match(Pid,Allocated)),
	if
		Cnt<2-> {{Free,[{Freq,Pid}|Allocated]},{ok,Freq}};
		true->{error,"one user limited to 3"}
	end.


dealloc({Free,Allocated},F,Pid)->
	case lists:member({F,Pid},Allocated) of
		true->
			NewAllocated = lists:keydelete(F,1,Allocated),
			{[F|Free],NewAllocated};
		false->
			io:format("wrong pid or freq~n"),
			{Free,Allocated}
	end.

getAllocated(Freq)-> element(2,Freq).

match(Value,[])->[];
match(Value,[{HKey,HValue}|L])->
	if
		HValue==Value->[HKey|match(Value,L)];
		true->match(Value,L)
	end.
