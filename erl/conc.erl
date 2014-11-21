-module(conc).
-export([color/0]).
-export([tell/2]).

color()	->
	receive
		{From,"blue"}	->	From ! "bluer",color();
		{From,"red"}	->	From ! "reder",color();
		{From,_}		->	From ! "anybody",color()
	end.

tell(To,Color)	->
	To ! {self(),Color},
	receive
		Trans	->	Trans
	end.
