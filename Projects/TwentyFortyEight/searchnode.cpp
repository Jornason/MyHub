#include "searchnode.h"

SearchNode::SearchNode()
{
  this->parent=nullptr;
  this->state=nullptr;
}
SearchNode::SearchNode(ProblemState &prob)
{
  this->state=prob;
  parent = nullptr;
}
SearchNode::SearchNode(SearchNode *orig)
{
  this->parent=orig;
  this->state=orig->state;
}
SearchNode::~SearchNode()
{
  //nothing to do
}
SearchNode SearchNode::operator=(const SearchNode&rhs)
{
  this->parent=rhs.parent;
  this->state=rhs.state;
  return *this;
}
bool SearchNode::operator ==(const SearchNode&other)const
{
  if(this==&other)
  {
    return true;
  }
  if(this->state!=other.state)return false;
  return true;
}
bool SearchNode::checkBacktrack()
{
  if(this->parent!=nullptr)
  {
    return this->checkAncestor(this->parent);
  }
  return false;
}
void SearchNode::printSolution()
{
  if(this->parent!=nullptr)
  {
    (*this->parent).printSolution();
  }
  this->state.printState();
}
SearchNode& SearchNode::getParent()
{
  return (*this->parent);
}
ProblemState& SearchNode::getState()
{
  return this->state;
}
void SearchNode::setParent(SearchNode &parent)
{
  this->parent=&parent;
}
void SearchNode::setState(ProblemState &state)
{
  this->state=state;
}
bool SearchNode::checkAncestor(SearchNode* that)
{
  if(this->state==that->state)
  {
    return true;
  }
  if(that->parent!=nullptr)
  {
    return this->checkAncestor(that->parent);
  }
  return false;
}

