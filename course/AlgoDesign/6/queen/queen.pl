valid_queen((Row,Col))	:-	member(Col,[1,2,3,4,5,6,7,8]).

valid_board([]).
valid_board([Head|Tail])	:-	valid_queen(Head),valid_board(Tail).

cols([],[]).
cols([(_,Col)|QueensTail],[Col|ColTail])	:-
	cols(QueensTail,ColTail).

diags1([],[]).
diags1([(Row,Col)|QueensTail],[Diag|DiagTail])	:-
	Diag is Col - Row,diags1(QueensTail,DiagTail).

diags2([],[]).
diags2([(Row,Col)|QueensTail],[Diag|DiagTail])	:-
	Diag is Col + Row,diags2(QueensTail,DiagTail).

eight_queen(Board):-
	Board=[(1,_),(2,_),(3,_),(4,_),(5,_),(6,_),(7,_),(8,_)],
	valid_board(Board),
	cols(Board,Cols),
	diags1(Board,Diags1),
	diags2(Board,Diags2),
	fd_all_different(Diags1),
	fd_all_different(Diags2),
	fd_all_different(Cols).
