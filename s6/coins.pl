:- use_module(library(clpfd)).

ejemplo(0,   26, [1,2,5,10]).  % Solution: [1,0,1,2]
ejemplo(1,  361, [1,2,5,13,17,35,157]).

calculateAmmount([], [], 0, 0).
calculateAmmount([C1|C], [V1|V], M, S) :-
	calculateAmmount(C, V, M1, S1),
	M #= M1 + C1*V1,
	S #= S1 + V1.

doExample(E) :-
	ejemplo(E, Amount, Coins),
    nl, write('Paying amount '), write(Amount), write(' using the minimal number of coins of values '), write(Coins), nl,
    length(Coins, N),
    length(Vars, N), % get list of N prolog vars    
    Vars ins 0..Amount,
    calculateAmmount(Coins, Vars, Money, Size),
    Money #= Amount,
    labeling([min(Size)], Vars),
    write(Vars), nl.

main:-
	doExample(0),
	doExample(1),
	halt.