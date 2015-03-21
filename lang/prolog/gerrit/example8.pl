submit_rule(S) :-
  gerrit:default_submit(X),
  % unpack (=..) X to submit and Ls
  X =.. [submit | Ls],
  add_non_author_approval(Ls, R),
  % pack back result to S as return value
  % =.. is *univ*, and works both way
  S =.. [submit | R].

add_non_author_approval(S1, S2) :-
  gerrit:commit_author(A),
  gerrit:commit_label(label('Code-Review', 2), R),
  % cut ! operator
  R \= A, !,
  S2 = [label('Non-Author-Code-Review', ok(R)) | S1].

add_non_author_approval(S1, [label('Non-Author-Code-Review', need(_)) | S1]).
