-module(lst).

filter([],N)->[];
filter(Lst,N)->
	if
		hd(Lst)=<N->[hd(Lst)|filter(tl(Lst),N)];
		true->filter(tl(Lst),N)
	end.

rev([])->[];
rev(Lst)->
	[rev(tl(Lst))|hd(Lst)].

conc([])->[];
conc(Lst)->
	if
		tl(Lst)==[]->Lst;
		true->[hd(Lst)++hd(tl(Lst))|tl(tl(Lst))]
	end.

