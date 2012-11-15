-module().
-behaviour(gen_server).
-export([start_link/0]).
-export([init/1,handle_call/3,handle_cast/2,handle_info/2,terminate/2,code_change/3]).

start_link()-> gen_server:start_link({local,?SERVER},?MODULE,[],[]).
init([]) -> {ok,}.

handle_call(_Request,_From,State)->{reply,Reply,State}.
handle_cast(_Msg,State)-> {noreply,State}.
handle_info(_Info,State)-> {noreply,State}.
terminate(_Reason,_State)->ok.
code_change(_OldVsn,State,Extra)->{ok,State}.

stop()->gen_server:call(?MODULE,stop).

new_account(Who) -> gen_server:call(?MODULE,{new,Who}).
deposit(Who,Amount) -> gen_server:call(?MODULE,{add,Who,Amount}).
withdraw(Who,Amount) -> gen_server:call(?MODULE,{remove,Who,Amount}).
