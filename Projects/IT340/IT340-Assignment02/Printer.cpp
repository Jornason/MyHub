#include"Printer.h"
Printer::Printer()
{
  this->tree=NULL;
}
Printer::Printer(DecisionTree*tree,ostream&oStream)
{
  this->tree=tree;
  //Print the root node
  this->printNode(this->tree->getRoot(),0,oStream);
}
Printer::Printer(Printer*orig)
{
  this->tree=orig->tree;
}
Printer Printer::operator=(const Printer&rhs)
{
  if(this==&rhs)
  {
    return*this;
  }
  this->tree=rhs.tree;
  return*this;
}
Printer::~Printer()
{
  //Nothing to do
}
void Printer::printNode(NodePtr&node,unsigned int indent,ostream&oStream)
{
  //Get the node's splitting attribute
  int splitAttr=node->getSplitAttr();
  unsigned int i,j;
  //Print the indent
  for(i=0;i<indent;i++)
  {
    oStream<<" ";
  }
  //Print the attribute name
  oStream<<this->tree->getAttr(splitAttr).getName()<<endl;
  //Print each possible value
  for(i=0;i<this->tree->getAttr(splitAttr).getNumVals();i++)
  {
    //Print the indent
    for(j=0;j<indent+1;j++)
    {
      oStream<<" ";
    }
    //Print the attribute value
    oStream<<"="<<this->tree->getAttr(splitAttr).getVal(i);
    if(node->getDecision(i)->getClass()!=-1)
    {
      //If the node was resolved, print the classification
      int nodeClass=node->getDecision(i)->getClass();
      oStream<<" => "<<this->tree->getClass(nodeClass)<<endl;
    }
    else
    {
      //Otherwise print the sub-node
      oStream<<endl;
      this->printNode(node->getDecision(i),indent+2,oStream);
    }
  }
}
