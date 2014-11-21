-module(inctime).
-export([run/0]).

run()->
	receive
		_->io:format("recvmsg~n")
	after 1000->
		io:format("1000~n"),
		run()
end.
