#include "problemstate.h"
ProblemState::ProblemState()
{
  this->cost=0;
}
ProblemState::ProblemState(ProblemState *orig)
{
  this->cost=orig->cost;
}

ProblemState::~ProblemState()
{
  //Nothing to do
}

int ProblemState::getCost()
{
  return this->cost;
}
void ProblemState::setCost(int cost)
{
  this->cost=cost;
}


