-module(ch3).
-export([sum/1,sum/2]).

%% -------------------------------------------------------------------%%
sum(N,M)->
	if
		N>M	->0;
		N==M->N;
		true	->N+sum(N+1,M)
	end.

sum(N)->
	sum(1,N).

%% -------------------------------------------------------------------%%
create(Begin,End,Pace)->
	if
		Pace==0->[];
		((Pace>0) and (Begin+Pace>End))-> [Begin];
		((Pace<0) and (Begin+Pace<End))-> [Begin];
		true-> [Begin|create(Begin+Pace,End,Pace)]
	end.

create(Begin,End)->
	if
		Begin==End->[Begin];
		Begin<End->create(Begin,End,1);
		Begin>End->create(Begin,End,-1)
	end.

create(Begin)->
	create(1,Begin).

reverse_create(Begin)->
	create(Begin,1).
%% -------------------------------------------------------------------%%
myprint(N,M,Pace)->
	if
		((N+Pace>M)) ->io:format("Number:~p~n",[N]);
		true->io:format("Number:~p~n",[N]),myprint(N+Pace,M,Pace)
	end.

myprint(N)->myprint(1,N,1).

print_even(N)->myprint(2,N,2).
