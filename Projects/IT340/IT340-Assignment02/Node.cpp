#include"Node.h"
Node::Node()
{
  //The node has no classification yet
  this->classIndx=-1;
  //The node has no splitting attribute yet
  this->splitAttr=-1;
}
Node::Node(Node*orig)
{
  this->decisionLst=orig->decisionLst;
  this->classIndx=orig->classIndx;
  this->splitAttr=orig->splitAttr;
}
Node Node::operator=(const Node&rhs)
{
  if(this==&rhs)
  {
    return*this;
  }
  this->decisionLst=rhs.decisionLst;
  this->classIndx=rhs.classIndx;
  this->splitAttr=rhs.splitAttr;
  return*this;
}
Node::~Node()
{
  //Nothing to do
}
void Node::addDecision(shared_ptr<Node>&node)
{
  this->decisionLst.push_back(node);
}
void Node::setClass(int classification)
{
  this->classIndx=classification;
}
void Node::setSplitAttr(int splitAttr)
{
  this->splitAttr=splitAttr;
}
shared_ptr<Node>&Node::getDecision(int index)
{
  return this->decisionLst[index];
}
int Node::getClass()
{
  return this->classIndx;
}
int Node::getSplitAttr()
{
  return this->splitAttr;
}
unsigned int Node::getNumDecisions()
{
  return this->decisionLst.size();
}
