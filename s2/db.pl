% Exercice 2
prod([], 1).
prod([X|L], P) :- prod(L, P2), P is P2 * X.

% Exercice 3
pescalar([], [], 0).
pescalar([X|L1], [Y|L2], P) :- pescalar(L1, L2, P2), P is P2 + X * Y.

% Exercice 4
intersection([], _, []).
intersection([X|L1], L2, [X|L3]) :- pert(X, L2), !, intersection(L1, L2, L3).
intersection([_|L1], L2, L3) :- intersection(L1, L2, L3).

union([], L, L).
union([X|L1], L2, L3) :- pert(X, L2), !, union(L1, L2, L3).
union([X|L1], L2, [X|L3]) :- union(L1, L2, L3).

% Exercice 5
last(L, X) :- append(_, [X], L).

reverse([], []).
reverse(L, [X|R]) :- append(L1, [X], L), reverse(L1, R).

% Exercice 6
fib(1, 1).
fib(2, 1).
fib(N, F) :- N > 2, N1 is N - 1, N2 is N - 2, fib(N1, F1), fib(N2, F2), F is F1 + F2.

% Exercice 7
dados(0, 0, []).
dados(P, N, [X|L]) :- N > 0, pert(X, [1,2,3,4,5,6]), P1 is P - X, N1 is N - 1, dados(P1, N1, L).

% Exercice 8
suma_demas(L) :- pert_con_resto(X, L, R), suma(R, X), !.

% Exercice 9
suma_ants(L) :- append(L1, [X|_], L), suma(L1, X), !.

% Exercice 11
esta_ordenada([]).
esta_ordenada([_]) :- !.
esta_ordenada([X,Y|L]) :- X >= Y, esta_ordenada([Y|L]).

% Exercice 12
ordenacion(L, O) :- permutation(L, O), esta_ordenada(O), !.

% Exercice 14
insertion_sort([], []).
insertion_sort([X|L], O) :- insertion_sort(L, L1), insertion(X, L1, O). 

insertion(X, [], [X]).
insertion(X, [Y|L], [X,Y|L]) :- X =< Y.
insertion(X, [Y|L], [Y|L1]) :- X > Y, insertion(X, L, L1).

% Exercice 16
merge_sort([], []) :- !.
merge_sort([X], [X]) :- !.
merge_sort(L, O) :- split(L, L1, L2), merge_sort(L1, L11), merge_sort(L2, L22), merge(L11, L22, O).

split([], [], []).
split([X], [X], []).
split([X,Y|L], [X|L1], [Y|L2]) :- split(L, L1, L2).

merge(L, [], L) :- !.
merge([], L, L).
merge([X|L1], [Y|L2], [X|L]) :- X =< Y, !, merge(L1, [Y|L2], L).
merge([X|L1], [Y|L2], [Y|L]) :- merge([X|L1], L2, L). 

% Exercice 18
palindrome(L) :- permutation(L, P), is_palindrome(P), write(P), nl, fail.
palindrome(_).

is_palindrome([]).
is_palindrome([_]) :- !.
is_palindrome([X|P]) :- append(L, X, P), is_palindrome(L). 

%%%%% UTILS %%%%
pert(X, [X|_]).
pert(X, [_|L]):- pert(X,L).

pert_con_resto(X, L, R) :- append(L1, [X|L2], L), append(L1, L2, R).

suma([], 0).
suma([X|L], S) :- suma(L, S1), S is S1+X.