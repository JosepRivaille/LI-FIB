:- use_module(library(clpfd)).
ejemplo(1, [0,0,8,2,6,4,5,3,7,0,0], [0,0,7,1,6,3,4,5,2,7,0,0] ).
ejemplo(2, [10,4,8,5,6], [5,3,4,0,5,0,5,2,2,0,1,5,1] ).
ejemplo(3, [11,5,4], [3,2,3,1,1,1,1,2,3,2,1] ).

doExample(E) :-
	ejemplo(E, RowSums, ColSums),
	length(RowSums, NumRows),
	length(ColSums, NumCols),
	NVars is NumRows * NumCols,
	length(L, NVars), L ins 0..1,
	matrixByRows(L, NumCols, MatrixByRows),
	transpose(MatrixByRows, MatrixByColumns),
	declareConstraints(RowSums, MatrixByRows),
	declareConstraints(ColSums, MatrixByColumns),
	label(L), pretty_print(RowSums, ColSums, MatrixByRows).

flatten([], []).
flatten([X|L], F) :- flatten(X, F1), flatten(L, F2), append(F1, F2, F), !.
flatten(X, [X]).

declareConstraints([], []) :- !.
declareConstraints([RS|RowSums], [Row|MatrixByRows]) :-
	sum(Row, #=, RS), declareConstraints(RowSums, MatrixByRows).

matrixByRows([], _, []) :- !.
matrixByRows(L, NumCols, MatrixByRows) :-
	length(Row, NumCols), append(Row, RestL, L),
	matrixByRows(RestL, NumCols, MatrixByRowsP),
	append([Row], MatrixByRowsP, MatrixByRows).

pretty_print(_,ColSums,_):- write('     '), member(S, ColSums), writef('%2r ',[S]), fail.
pretty_print(RowSums,_,M):- nl,nth1(N,M,Row), nth1(N,RowSums,S), nl, writef('%3r   ',[S]), member(B,Row), wbit(B), fail.
pretty_print(_,_,_) :- nl, nl.
wbit(1):- write('*  '),!.
wbit(0):- write('   '),!.

main :-
	doExample(1),
	doExample(2),
	doExample(3),
	halt.
