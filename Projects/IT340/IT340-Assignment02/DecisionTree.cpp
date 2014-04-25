#include"DecisionTree.h"
DecisionTree::DecisionTree()
{
  this->root=NodePtr(new Node());
}
DecisionTree::DecisionTree(DecisionTree*orig)
{
  this->root=orig->root;
  this->attrLst=orig->attrLst;
}
DecisionTree DecisionTree::operator=(const DecisionTree&rhs)
{
  if(this==&rhs)
  {
    return*this;
  }
  this->root=rhs.root;
  this->attrLst=rhs.attrLst;
  return*this;
}
DecisionTree::~DecisionTree()
{
  //Nothing to do
}
void DecisionTree::addAttr(shared_ptr<Attribute>&attr)
{
  this->attrLst.push_back(attr);
}
NodePtr&DecisionTree::getRoot()
{
  return this->root;
}
Attribute&DecisionTree::getAttr(unsigned int index)
{
  return*this->attrLst[index];
}
string&DecisionTree::getClass(unsigned int index)
{
  return this->attrLst[this->attrLst.size()-1]->getVal(index);
}
unsigned int DecisionTree::getNumAttrs()
{
  return this->attrLst.size();
}
unsigned int DecisionTree::getNumClasses()
{
  return this->attrLst[this->attrLst.size()-1]->getNumVals();
}
