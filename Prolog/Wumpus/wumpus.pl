%Thomas Lyons
%Wumpus Agent
%Program 3
%Completed 3/31/2014 at 4:36am
%Estimated Success rate: Approximately 54%

:- dynamic ([
    %The agent's coordinates on the map
    agent_loc/1,
    %The agent's direction (north,south,east,west)
    agent_direction/1,
    %The places where there is no gold
    notGold/1,
    %The cells that have been visited
    visited_cells/1,
    %The places where there is no wumpus
    notWumpus/1,
    %The places where there is no pit
    notPit/1,
    %The locations that mark that bounds of the map
    edge/1,
    %What navigation/avoidance strategy are we taking?
    strategy/1,
    %Which direction is the current sweep
    sweepDirection/1,
    %The path the agent has taken (excludes dead ends)
    agentPath/1
    ]).

%Initialize the agent for the next test
	init_agent :-
		%Get amnesia
		retractall(agent_loc(_)),
		retractall(agent_direction(_)),
		retractall(notGold(_)),
		retractall(visited_cells(_)),
		retractall(notWumpus(_)),
		retractall(notPit(_)),
		retractall(edge(_)),
		retractall(strategy(_)),
		retractall(sweepDirection(_)),
		retractall(agentPath(_)),
		%Start at location (1,1)
		assert(agent_loc([1,1])),
		%start by facing east
		assert(agent_direction(east)),
		%Gold could be anywhere
		assert(notGold([])),
		%We have only visited the first cell
		assert(visited_cells([[1,1]])),
		%Nowhere is safe
		assert(notWumpus([[1,1]])),
		assert(notPit([[1,1]])),
		%We know the edges already
		assert(edge([[0,_],[9,_],[_,0],[_,9]])),
		%We are not in the process of a turn
		assert(strategy(noTurn)),
		%We are going to begin the sweep east
		assert(sweepDirection(east)),
		%We include the origin in the path
		assert(agentPath([[1,1]])).

%Give the agent a list of perceptions and ask for an action
	run_agent( Percept, Action ) :-
		%Where are we and what direction are we facing?
		agent_loc(Loc),
		agent_direction(AD),
		%What was our current strategy?
		strategy(Strat),
		sweepDirection(Sweep),
		%Remember what we saw here
		update_KB(Percept,Loc,AD),
		%What should we do?
		ask_KB(Loc,AD,Strat,Sweep,Action).

%Cross a cell off the list
	visit(Loc) :-
		agentPath(AP),
		retractall(agentPath(_)),
		assert(agentPath([Loc|AP])),
		visited_cells(Z),
		retractall(visited_cells(_)),
		assert(visited_cells([Loc|Z])).

%The wumpus is dead (never actually used in this implementation
	update_KB([Stench,Breeze,Glitter,Bump,yes,_,_],Loc,AD) :-
		%Mark possible pit locations
		add_pit_KB(Breeze,Loc),
		%Remove all wumpus locations
		remove_wumpus_KB.
 
%Silence 
	update_KB([Stench,Breeze,Glitter,Bump,no,_,_],Loc,AD) :-
		%Mark all possible pit locations
		add_pit_KB(Breeze,Loc),
		%Mark all possible wumpus locations
		add_wumpus_KB(Stench,Loc),
		%Mark all possible gold locations
		add_gold_KB(Glitter,Loc).

%Mark pit Locations
	add_pit_KB(Breeze,[X,Y]) :-
		Z1 is Y+1,
		Z2 is Y-1,
		Z3 is X+1,
		Z4 is X-1,
		assume_pit(Breeze,[X,Z1]),
		assume_pit(Breeze,[X,Z2]),
		assume_pit(Breeze,[Z3,Y]),
		assume_pit(Breeze,[Z4,Y]).
    
    
%Mark wumpus locations
	add_wumpus_KB(Stench,[X,Y]) :-
		Z1 is Y+1,
		Z2 is Y-1,
		Z3 is X+1,
		Z4 is X-1,
		assume_wumpus(Stench,[X,Z1]),
		assume_wumpus(Stench,[X,Z2]),
		assume_wumpus(Stench,[Z3,Y]),
		assume_wumpus(Stench,[Z4,Y]).
    
%Mark gold locations
	add_gold_KB(no,[X,Y]) :-
		notGold(G),
		retractall(notGold(_)),
		assert(notGold([[X,Y]|G])).
%If there is glitter, then don't add it to the notGold list
	add_gold_KB(yes,[X,Y]) :-
		notGold(G).
    
%The wumpus is dead, so we can stop tracking it (never actually used in this
%implementation)
	remove_wumpus_KB :-
		retractall(notWumpus(_)),
		assert(notWumpus(_)).

%Assume that a pit is not at a location
	assume_pit(no,Loc) :-
		notPit(P),
		retractall(notPit(_)),
		assert(notPit([Loc|P])).
%If there is a breeze, then don't add it to the notPit list
	assume_pit(yes,Loc):-
		notPit(P).

%Assume that a wumpus is not as a location
	assume_wumpus(no,Loc) :-
		notWumpus(W),
		retractall(notWumpus(_)),
		assert(notWumpus([Loc|W])).
%If there is a stench, then don't add it to the notWumpus list
	assume_wumpus(yes,Loc) :-
		notWumpus(W).

%Find the direction to backtrack to (we know where we are and where we want to
%go, just find which direction that is in)
	findDirection([X,Y],[VH|[[X1,Y1]|VT]],Direction) :-
		X = X1,
		Y > Y1,
		Direction = south.
	findDirection([X,Y],[VH|[[X1,Y1]|VT]],Direction) :-
		X = X1,
		Y < Y1,
		Direction = north.
	findDirection([X,Y],[VH|[[X1,Y1]|VT]],Direction) :-
		X > X1,
		Y = Y1,
		Direction = west.
	findDirection([X,Y],[VH|[[X1,Y1]|VT]],Direction) :-
		X < X1,
		Y = Y1,
		Direction = east.

%Based on the findDirection result, find which action will set us on the right
%course
%Go forward from where you are
	findAction([X,Y],west,west,Action,[V1H|V1T]) :-
		Action = goforward,
		X1 is X-1,
		retractall(agentPath(_)),
		assert(agentPath(V1T)),
		retractall(agent_loc(_)),
		assert(agent_loc([X1,Y])).
	findAction([X,Y],east,east,Action,[V1H|V1T]) :-
		Action = goforward,
		X1 is X+1,
		retractall(agentPath(_)),
		assert(agentPath(V1T)),
		retractall(agent_loc(_)),
		assert(agent_loc([X1,Y])).
	findAction([X,Y],north,north,Action,[V1H|V1T]) :-
		Action = goforward,
		Y1 is Y+1,
		retractall(agentPath(_)),
		assert(agentPath(V1T)),
		retractall(agent_loc(_)),
		assert(agent_loc([X,Y1])).
	findAction([X,Y],south,south,Action,[V1H|V1T]) :-
		Action = goforward,
		Y1 is Y-1,
		retractall(agentPath(_)),
		assert(agentPath(V1T)),
		retractall(agent_loc(_)),
		assert(agent_loc([X,Y1])).
%Make a turn to face the correct direction (always turn left)
	findAction([X,Y],west,D,Action,V) :-
		Action = turnleft,
		retractall(agent_direction(_)),
		assert(agent_direction(south)).
	findAction([X,Y],east,D,Action,V) :-
		Action = turnleft,
		retractall(agent_direction(_)),
		assert(agent_direction(north)).
	findAction([X,Y],north,D,Action,V) :-
		Action = turnleft,
		retractall(agent_direction(_)),
		assert(agent_direction(west)).
	findAction([X,Y],south,D,Action,V) :-
		Action = turnleft,
		retractall(agent_direction(_)),
		assert(agent_direction(east)).

%Escape the dungeon
%At the origin, so just climb
	ask_KB([X,Y],AD,escape,Sweep,Action) :-
		X = 1,
		Y = 1,
		Action = climb.
%Otherwise, backtrack along the path we have taken
	ask_KB(Loc,AD,escape,Sweep,Action) :-
		agentPath(V),
		findDirection(Loc,V,Direction),
		findAction(Loc,AD,Direction,Action,V).

%The process of turning around for backtracking (we have to face the direction
%that we are traveling
%In step one of turning around (always turn left)
	%Facing north
		ask_KB([X,Y],north,turnAroundStep1,Sweep,Action) :-
			retractall(agent_direction(_)),
			assert(agent_direction(west)),
			retractall(strategy(_)),
			assert(strategy(turnAroundStep2)),
			Action=turnleft.
	%Facing south
		ask_KB([X,Y],south,turnAroundStep1,Sweep,Action) :-
			retractall(agent_direction(_)),
			assert(agent_direction(east)),
			retractall(strategy(_)),
			assert(strategy(turnAroundStep2)),
			Action=turnleft.
	%Facing east
		ask_KB([X,Y],east,turnAroundStep1,Sweep,Action) :-
			retractall(agent_direction(_)),
			assert(agent_direction(north)),
			retractall(strategy(_)),
			assert(strategy(turnAroundStep2)),
			Action=turnleft.
	%Facing west
		ask_KB([X,Y],west,turnAroundStep1,Sweep,Action) :-
			retractall(agent_direction(_)),
			assert(agent_direction(south)),
			retractall(strategy(_)),
			assert(strategy(turnAroundStep2)),
			Action=turnleft.

%Step two of the process of turning around (we are facing the correct
%direction)
    %At the origin and nowhere to go, so just escape
		ask_KB([X,Y],AD,turnAroundStep2,Sweep,Action) :-
			X = 1,
		    Y = 1,
		    Action = climb.
    %Otherwise, progress forward into the backtracking process
		ask_KB([X,Y],AD,turnAroundStep2,Sweep,Action) :-
			retractall(strategy(_)),
			assert(strategy(backtrack)),
			agentPath([APH|[APH2|APT]]),
			retractall(agentPath(_)),
			assert(agentPath([APH2|APT])),
			retractall(agent_loc(_)),
			assert(agent_loc(APH2)),
			Action=goforward.

%Backtracking
	%Facing West
		%We can visit the cell in front
			ask_KB([X,Y],west,backtrack,Sweep,Action) :-
				X1 is X-1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_loc(_)),
				assert(agent_loc([X1,Y])),
				visit([X1,Y]),
				Action=goforward.
		%We can visit the cell to the south
			ask_KB([X,Y],west,backtrack,Sweep,Action) :-
				Y1 is Y-1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(south)),
				Action=turnleft.
		%We can visit the cell to the north
			ask_KB([X,Y],west,backtrack,Sweep,Action) :-
				Y1 is Y+1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(north)),
				Action=turnright.
	%Facing East
		%We can visit the cell in front
			ask_KB([X,Y],east,backtrack,Sweep,Action) :-
				X1 is X+1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_loc(_)),
				assert(agent_loc([X1,Y])),
				visit([X1,Y]),
				Action=goforward.
		%We can visit the cell to the south
			ask_KB([X,Y],east,backtrack,Sweep,Action) :-
				Y1 is Y-1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(south)),
				Action=turnright.
		%We can visit the cell to the north
			ask_KB([X,Y],east,backtrack,Sweep,Action) :-
				Y1 is Y+1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(north)),
				Action=turnleft.
	%Facing South
		%We can visit the cell in front
			ask_KB([X,Y],south,backtrack,Sweep,Action) :-
				Y1 is Y-1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_loc(_)),
				assert(agent_loc([X,Y1])),
				visit([X,Y1]),
				Action=goforward.
		%We can visit the cell to the east
			ask_KB([X,Y],south,backtrack,Sweep,Action) :-
				X1 is X+1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(east)),
				Action=turnleft.
		%We can visit the cell to the west
			ask_KB([X,Y],south,backtrack,Sweep,Action) :-
				X1 is X-1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(west)),
				Action=turnright.
	%Facing North
		%We can visit the cell in front
			ask_KB([X,Y],north,backtrack,Sweep,Action) :-
				Y1 is Y+1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_loc(_)),
				assert(agent_loc([X,Y1])),
				visit([X,Y1]),
				Action=goforward.
		%We can visit the cell to the east
			ask_KB([X,Y],north,backtrack,Sweep,Action) :-
				X1 is X+1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(east)),
				Action=turnright.
		%We can visit the cell to the west
			ask_KB([X,Y],north,backtrack,Sweep,Action) :-
				X1 is X-1,
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(strategy(_)),
				assert(strategy(noTurn)),
				retractall(agent_direction(_)),
				assert(agent_direction(west)),
				Action=turnleft.
    %No suitable path found
		%We must continue to backtrack
			ask_KB(Loc,AD,backtrack,Sweep,Action) :-
				agentPath(V),
				findDirection(Loc,V,Direction),
				findAction(Loc,AD,Direction,Action,V).

%Grab the gold
	ask_KB(Loc,AD,Strat,Sweep,Action) :-
		notGold(G),
		not_member(Loc,G),
		retractall(notGold(_)),
		assert(notGold([Loc|G])),
		retractall(strategy(_)),
		assert(strategy(escape)),
		Action = grab. 

%Standard path progression (not escaping, not backtracking)
	%Facing east
		%nothing in front
			ask_KB([X,Y],east,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				X1 is X+1,
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(agent_loc(_)),
				assert(agent_loc([X1,Y])),
				visit([X1,Y]),
				Action=goforward.
		%Something in front but nothing to the south
			ask_KB([X,Y],east,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				Y1 is Y-1,
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(agent_direction(_)),
				assert(agent_direction(south)),
				Action=turnright.
		%Something in front and something to the south but nothing to the north
			ask_KB([X,Y],east,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				Y1 is Y+1,
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(agent_direction(_)),
				assert(agent_direction(north)),
				Action=turnleft.
		%We cannot continue in the current direction, begin backtracking
			ask_KB([X,Y],east,Strat,Sweep,Action) :-
				retractall(agent_direction(_)),
				assert(agent_direction(north)),
				retractall(strategy(_)),
				assert(strategy(turnAroundStep1)),
				Action=turnleft.
	%Facing west
		%Nothing in front
			ask_KB([X,Y],west,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				X1 is X-1,
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(agent_loc(_)),
				assert(agent_loc([X1,Y])),
				visit([X1,Y]),
				Action=goforward.
		%Something in front but nothing to the south
			ask_KB([X,Y],west,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				Y1 is Y-1,
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(agent_direction(_)),
				assert(agent_direction(south)),
				Action=turnleft.
		%Something in front and something to the south but nothing to the north
			ask_KB([X,Y],west,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				Y1 is Y+1,
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(agent_direction(_)),
				assert(agent_direction(north)),
				Action=turnright.
		%We cannot continue in the current direction, begin backtracking
			ask_KB([X,Y],west,Strat,Sweep,Action) :-
				retractall(agent_direction(_)),
				assert(agent_direction(south)),
				retractall(strategy(_)),
				assert(strategy(turnAroundStep1)),
				Action=turnleft.

	%Facing north
		%Nothing in front
			ask_KB([X,Y],north,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				Y1 is Y+1,
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(agent_loc(_)),
				assert(agent_loc([X,Y1])),
				visit([X,Y1]),
				Action=goforward.
		%Something in front but nothing to the west
			ask_KB([X,Y],north,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				X1 is X-1,
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(agent_direction(_)),
				assert(agent_direction(west)),
				Action=turnleft.
		%Something in front and something to the west but nothing to the east
			ask_KB([X,Y],north,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				X1 is X+1,
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(agent_direction(_)),
				assert(agent_direction(east)),
				Action=turnright.
		%We cannot continue in the current direction, begin backtracking
			ask_KB([X,Y],north,Strat,Sweep,Action) :-
				retractall(agent_direction(_)),
				assert(agent_direction(west)),
				retractall(strategy(_)),
				assert(strategy(turnAroundStep1)),
				Action=turnleft.

	%Facing south
		%Nothing in front
			ask_KB([X,Y],south,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				Y1 is Y-1,
				not_member([X,Y1],E),
				member([X,Y1],P),
				member([X,Y1],W),
				not_member([X,Y1],V),
				retractall(agent_loc(_)),
				assert(agent_loc([X,Y1])),
				visit([X,Y1]),
				Action=goforward.
		%Facing south and something in front but nothing to the west
			ask_KB([X,Y],south,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				X1 is X-1,
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(agent_direction(_)),
				assert(agent_direction(west)),
				Action=turnright.
		%Facing south and something in front and something to the west but nothing
    %to the east
			ask_KB([X,Y],south,Strat,Sweep,Action) :-
				notPit(P),
				notWumpus(W),
				edge(E),
				visited_cells(V),
				X1 is X+1,
				not_member([X1,Y],E),
				member([X1,Y],P),
				member([X1,Y],W),
				not_member([X1,Y],V),
				retractall(agent_direction(_)),
				assert(agent_direction(east)),
				Action=turnleft.
		%We cannot continue in the current direction, begin backtracking
			ask_KB([X,Y],south,Strat,Sweep,Action) :-
				retractall(agent_direction(_)),
				assert(agent_direction(east)),
				retractall(strategy(_)),
				assert(strategy(turnAroundStep1)),
				Action=turnleft.


%X is not a member of the list
	not_member(X, []).
	not_member(X, [Y|T]) :-
		X=Y -> fail;
		not_member(X, T).
