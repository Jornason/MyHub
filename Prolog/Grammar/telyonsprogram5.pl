%Thomas Lyons
%IT340 Program 5
%Completed 3:22am on 4/29/2014
%Sentence grammar checker

%All utterances must be sentences
utterance(X) :- sentence(X,[]).

%A sentence must always have a noun phrase and a verb phrase
%Optionally, prepositional phrase may be occur after the noun phrase, verb phrase, or both
%Also, a noun phrase may occur after a verb phrase
%There is a special case for forms of "to be"
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  tobephrase(Rest,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  tobephrase(Rest,Rest2),
  prepphrase(Rest2,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  adverbverbphrase(Rest,Rest2),
  tobephrase(Rest2,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  adverbverbphrase(Rest,Rest2),
  tobephrase(Rest2,Rest3),
  prepphrase(Rest3,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  prepphrase(Rest,Rest2),
  tobephrase(Rest2,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  prepphrase(Rest,Rest2),
  tobephrase(Rest2,Rest3),
  prepphrase(Rest3,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  prepphrase(Rest,Rest2),
  adverbverbphrase(Rest2,Rest3),
  tobephrase(Rest3,End).
sentence(Start,End):-
  nounphrase(Start,Rest),
  prepphrase(Rest,Rest2),
  adverbverbphrase(Rest2,Rest3),
  tobephrase(Rest3,Rest4),
  prepphrase(Rest4,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, Rest2),
  nounphrase(Rest2,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, Rest3),
  nounphrase(Rest3,Rest4),
  prepphrase(Rest4,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, Rest3),
  nounphrase(Rest3,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, Rest2),
  prepphrase(Rest2,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, Rest3),
  prepphrase(Rest3,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, Rest2),
  nounphrase(Rest2,Rest3),
  prepphrase(Rest3,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, Rest2),
  prepphrase(Rest2,Rest3),
  nounphrase(Rest3,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, Rest2),
  prepphrase(Rest2,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
	verbphrase(Rest, Rest2),
  prepphrase(Rest2,Rest3),
  nounphrase(Rest3,Rest4),
  prepphrase(Rest4,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, Rest3),
  prepphrase(Rest3,End).
sentence(Start, End) :-
	nounphrase(Start, Rest),
  prepphrase(Rest,Rest2),
	verbphrase(Rest2, Rest3),
  prepphrase(Rest3,Rest4),
  nounphrase(Rest4,Rest5),
  prepphrase(Rest5,End).
  

%A prepositional phrase contains a preposition and a noun phrase
%They may be stacked
prepphrase(Prep, End) :-
	getprep(Prep,Rest),
	nounphrase(Rest, End).
prepphrase(Prep, End) :-
	getprep(Prep,Rest),
	nounphrase(Rest, Rest2),
  prepphrase(Rest2,End).

%Get a single peoposition
getprep([Prep|End],End):-
  prep(Prep).

%A noun phrase must contain an article and a noun. It may also optionally contain a series of
%adjectives
nounphrase(Article,End) :-
  getarticle(Article,Rest),
  getnoun(Rest,End).
nounphrase(Article,End) :-
  getarticle(Article,Rest),
  adjectivephrase(Rest,Rest2),
  getnoun(Rest2,End).

%Get a single article
getarticle([Article|End],End):-
  article(Article).

%Get a single noun
getnoun([Noun|End],End):-
  noun(Noun).
getnoun(Noun,[]):-
  noun(Noun).

%An adjective phrase consists of an adjective
%It ma also consist of a series of adverbs.
%This statement can be stacked.
adjectivephrase([Adjective|End],End):-
  adjective(Adjective).
adjectivephrase([Adverb|Adjective],End):-
  adverbadj(Adverb),
  adjectivephrase(Adjective,End).
adjectivephrase(Adjective,End):-
  getadjective(Adjective,Rest),
  adjectivephrase(Rest,End).
  
%Adverb phrase (for adjectives)
%Consists of an adverb. Can be stacked
adverbadjphrase([Adverb|End],End):-
  adverbadj(Adverb).
adverbadjphrase([Adverb|Adverb2],End):-
  adverbadj(Adverb),
  adverbadjphrase(Adverb2,End).

%A verb phrase must contain a verb. It may also contain a series of adverbs
verbphrase([Verb|End],End) :-
	verb(Verb).
verbphrase(Verb,[]):-
  verb(Verb).
verbphrase(Adverb, End) :-
  adverbverbphrase(Adverb,Rest),
  getverb(Rest,End).
verbphrase(Adverb, []) :-
  adverbverbphrase(Adverb,Rest),
  getverb(Rest,[]).
  
%To be phrase
%Contains a form of "to be" as well as an adjective. May also contain a series of adverbs in
%front of the adjective
tobephrase([Verb|Adjective],[]) :-
  tobe(Verb),
  getadjective(Adjective,[]).
tobephrase([Verb|Adverb],[]) :-
  tobe(Verb),
  adverbadjphrase(Adverb,Rest),
  getadjective(Rest,[]).
tobephrase([Verb|Adjective],End) :-
  tobe(Verb),
  getadjective(Adjective,End).
tobephrase([Verb|Adverb],End) :-
  tobe(Verb),
  adverbadjphrase(Adverb,Rest),
  getadjective(Rest,End).
tobephrase([Verb|Prep],[]) :-
  tobe(Verb),
  prepphrase(Prep,[]).
tobephrase([Verb|Adverb],[]) :-
  tobe(Verb),
  adverbadjphrase(Adverb,Rest),
  prepphrase(Rest,[]).
tobephrase([Verb|Prep],End) :-
  tobe(Verb),
  prepphrase(Prep,End).
tobephrase([Verb|Adverb],End) :-
  tobe(Verb),
  adverbadjphrase(Adverb,Rest),
  prepphrase(Rest,End).

%Get a single verb
getverb([Verb|End],End):-
  verb(Verb).

%Get a single adjective
getadjective([Adjective|End],End):-
  adjective(Adjective).
getadjective(Adjective,[]):-
  adjective(Adjective).


%Adverb phrase(for verbs)
%Consists of an adverb. Can be stacked
adverbverbphrase([Adverb|End],End):-
  adverbverb(Adverb).
adverbverbphrase([Adverb|Adverb2],End):-
  adverbverb(Adverb),
  adverbverbphrase(Adverb2,End).
adverbverbphrase(Adverb,End):-
  adverbadjphrase(Adverb,Rest),
  adverbverbphrase(Rest,End).
adverbverbphrase(Adverb,End):-
  adverbadjphrase(Adverb,Rest),
  adverbverbphrase(Rest,End).

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
adverbverb(slowly).
adverbverb(quickly).
adverbverb(carefully).

%Adverbs that can be attached to adjectives
adverbadj(very).
adverbadj(slightly).
adverbadj(extremely).

%Forms of "to be"
tobe(is).
tobe(was).

%Prepositions
prep(on).
prep(underneath).
prep(beside).


	  