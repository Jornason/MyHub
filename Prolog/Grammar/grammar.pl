% starting point for program 5

utterance(X) :- sentence(X,[]).

sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, End).

nounphrase([Article, Noun | End], End) :-
	article(Article),
	noun(Noun).

verbphrase([Verb|End], End) :-
	verb(Verb).
verbphrase([Verb|Rest], End) :-
	verb(Verb),
	nounphrase(Rest, End).

article(a).
article(the).

noun(man).
noun(dog).

verb(likes).
verb(bites).
	  