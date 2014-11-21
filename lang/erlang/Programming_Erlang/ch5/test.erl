-module(test).

test(Var,List)->
	Rt=lists:member(Var,List),
	case lists:member(Var,List) of
		%lists:member(Var,List)->
		%	io:format("bingo!");
		false->
			io:format("lose!");
		true->
			io:format("bingo!")
	end,
	Rt.
