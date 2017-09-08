# ref: https://stackoverflow.com/questions/9134380/how-to-access-list-permutations-in-prolog
takeout(X, [X | R], R).
takeout(X, [F | R], [F | S]) :- takeout(X, R, S).

perm([X | Y], Z) :- perm(Y, W), takeout(X, Z, W).
perm([],[]).
