% Wumpus World Simulator v2.3
%
% Written by Larry Holder (holder@cse.uta.edu)
%
%  Modified by Mary Elaine Califf to work with SWI Prolog
%
% A Prolog implementation of the Wumpus world described in Russell and
% Norvig's "Artificial Intelligence: A Modern Approach", Section 6.2.
% A few enhancements have been added:
%   - the wumpus can move according to simple rules (e.g., move towards the
%     gold location)
%   - the percept includes a binary image of the square the agent is
%     facing containing bitmaps of a wumpus, gold and/or pit
%   - the percept includes a natural language "hint"
%   - random wumpus world generator
%
% See comments on the following interface procedures:
%
%   evaluate_agent(Trials,Score,Time)
%   evaluate_agent2(Trials,Score,Time)
%   initialize(World,Percept)
%   restart(Percept)


:- ensure_loaded(library(lists)).    % Sicstus Prolog's list library
:- ensure_loaded('image.pl').        % Image percept generation
:- ensure_loaded('move_wumpus.pl').  % Wumpus movement
:- ensure_loaded('nl_hint.pl').      % Natural language hint percept 

:- use_module(library(jpl)).


:- dynamic([
  ww_initial_state/1,
  wumpus_world_extent/1,
  wumpus_location/2,
  wumpus_health/1,
  wumpus_orientation/1,
  wumpus_last_action/1,
  wumpus_movement_rule/1,
  gold/2,
  pit/2,
  agent_location/2,
  agent_orientation/1,
  agent_in_cave/1,
  agent_health/1,
  agent_gold/1,
  agent_arrows/1,
  agent_score/1,
  start_time/3,
  cave/1.
  ]).


gold_probability(10).  % Probability that a location has gold
pit_probability(15).   % Probability that a non-(1,1) location has a pit
max_agent_actions(200).  % Maximum actions per trial allowed by agent
max_agent_tries(10).     % Maximum agent tries (climb or die) per world


:-jpl_call('World','helloworld',[],_).
