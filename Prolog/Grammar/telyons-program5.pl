%Thomas Lyons
%IT340 Program 5
%Completed 2:42am on 4/28/2014
%Sentence grammar checker

%All utterances must be sentences
utterance(X) :- sentence(X,[]).

%A sentence must always have a noun phrase and a verb phrase
%Optionally, prepositional phrase may be occur after the noun phrase, verb phrase, or both
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, End).
sentence(Start, End) :-
	nounphrase(Start,Rest),
	verbphrase(Rest,Rest2),
  prepphrase(Rest2,End).
sentence(Start,End) :-
	nounphrase(Start,Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2,Rest3),
  prepphrase(Rest3,End).

%A prepositional phrase contains a preposition and a noun phrase
prepphrase([Prep|Rest], End) :-
	prep(Prep),
	nounphrase(Rest, End).

%A noun phrase must contain an article and a noun. It may also optionally contain up to 3
%adjectives which may each optionally have an adverb attached
nounphrase([Article,Noun| End], End) :-
	%articlePhrase(Article,Rest),
  article(Article),
	noun(Noun).
nounphrase([Article,Adjective,Noun|End],End):-
  article(Article),
  adjective(Adjective),
  noun(Noun).
nounphrase([Article,Adjective|End],End) :-
  %articlePhrase(Article,Rest),
  article(Article),
  adjectivePhrase(Adjective,End).

  
adjectivePhrase([Adjective,Noun,End],End):-
  adjective(Adjective),
  noun(Noun).
adjectivePhrase([Adjective|End],End):-
  adverbAdjPhrase(Adjective,End).
adjectivePhrase([Adjective|End],End):-
  adjectivePhrase(Adjective,End).
  
articlePhrase([Article,End],End):-
  article(Article).

adverbAdjPhrase([Adverb|Adjective],Adjective):-
  adverbAdj(Adverb).
adverbAdjPhrase([Adverb|Adjective],Adjective):-
  adverbAdjPhrase(Adverb,Rest),
  adjectivePhrase(Rest,Adjective).

%A verb phrase must contain a verb. It may optionally contain a noun phrase after the verb
%The verb may also have an adverb attached in front of it
%There is another kind of verb phrase that uses a form of "to be" and contains and adjective (and
%optionally, an adverb).
verbphrase([Verb|End], End) :-
	verb(Verb).
verbphrase([Verb|Rest], End) :-
	verb(Verb),
	nounphrase(Rest, End).
verbphrase([Adverb|End], End) :-
  adverbVerbPhrase(Adverb,End).
verbphrase([Adverb|Rest], End) :-
	adverbVerbPhrase(Adverb,Rest),
	nounphrase(Rest, End).
verbphrase([Verb|End],End) :-
  toBePhrase(Verb,Rest),
  adverbAdjPhrase(Rest,End).
  
toBePhrase([Verb|End],End):-
  toBe(Verb).
  
adverbVerbPhrase([Adverb|Adjective],Adjective):-
  adverbVerb(Adverb).
adverbVerbPhrase([Adverb|Adjective],Adjective):-
  adverbVerbPhrase(Adverb,Rest),
  adjectivePhrase(Rest,Adjective).

%The Articles
article(a).
article(the).

%The Nouns
noun(man).
noun(dog).
noun(cat).
noun(sidewalk).
noun(couch).

%The Verbs
verb(likes).
verb(hates).
verb(bites).

%The adjectives
adjective(big).
adjective(fat).
adjective(sleepy).

%Adverbs that can be attached to verbs
adverbVerb(slowly).
adverbVerb(quickly).
adverbVerb(carefully).

%Adverbs that can be attached to adjectives
adverbAdj(very).
adverbAdj(slightly).
adverbAdj(extremely).

%Forms of "to be"
toBe(is).
toBe(was).

%Prepositions
prep(on).
prep(underneath).
prep(beside).


	  