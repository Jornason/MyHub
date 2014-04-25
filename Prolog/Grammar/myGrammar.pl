
utterance(X) :-
  set_prolog_flag(toplevel_print_options, [quoted(true), portray(true), max_depth(100)]),
  sentence(X, []).
 

sentence(Start, End) :-
  nounphrase(Start, Rest),
  verbphrase(Rest, End).
  
nounphrase([Article, Adjective, Noun | End], End) :-
  article(Article),
  adjectivePhrase(Adj),
  powerSet(Adj, Adjective),
  %adverbAdjPhrase(Adjective),
  noun(Noun).

verbphrase([Verb | End], End) :-
	verb(Verb).
verbphrase([Verb, Adverb | End], End) :-
  verb(V),
  adverbVerbList(Adv),
  member(Adverb, Adv).
verbphrase([Verb, Adjective | End], End) :-
  toBe(Verb),
  adjectivePhrase(Adj),
  member(Adjective, Adj).
verbphrase([Verb | Rest], End) :-
  verb(Verb),
 nounphrase(Rest, End).
verbphrase([Verb, Adverb | Rest], End) :-
  verb(V),
  adverbVerbList(Adv),
  member(Adverb, Adv),
  nounphrase(Rest, End).

article(a).
article(the).

noun(man).
noun(dog).

verb(likes).
verb(bites).

toBe(is).
toBe(was).

adjectivePhrase(Adjective) :-
  adjective(Adj),
  member(Adjective, Adj).
adjectivePhrase(Adjective) :-
  adjective(Adj),
  adverbAdjPhrase(Adv),
  %adverbAdjList(Adv2),
  %product(Adv, Adv2, Adverb),
  %powerSet(Adv, AdverbPowerSet),
  %product(AdverbPowerSet, Adj, Adjective).
  product(Adv, Adj, Adjective).
  
adverbAdjPhrase(Adverb) :-
  adverbAdjList(Adv),
  powerSet(Adv, Adverb).


adjective([big,fat,sleepy]).
adverbAdjList([very, slightly]).
adverbVerbList([quickly, slowly]).

powerSet([], []).
powerSet([_ | T], P) :-
  powerSet(T, P).
powerSet([H | T], [H | P]) :-
  powerSet(T, P).
  
product([],_,[]).
product([H1|T1],L2,R):-
  put(H1,L2,R1),
  product(T1,L2,R2),
  append(R1,R2,R).

put(X,[],[]).
put(X,[H|T],[[X,H]|R]):-
  put(X,T,R).
  
not_member(X, []).
not_member(X, [Y|T]) :-
  X=Y -> fail;
  not_member(X, T).

