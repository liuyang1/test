likes(wallace,cheese).
likes(grommit,cheese).
likes(wendoln,sheep).

friend(X,Y)	:-	\+(X = Y),likes(X,Z),likes(Y,Z).
