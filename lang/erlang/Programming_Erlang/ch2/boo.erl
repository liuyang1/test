-module(boo).
-export([b_xor/2]).

b_not(false)->true;
b_not(true)->false.

b_and(true,true)->true;
b_and(_,_)->false.

b_or(false,false)->false;
b_or(_,_)->true.

b_nand(A,B)->b_not(b_and(A,B)).

b_xor(A,B)->A==B.
