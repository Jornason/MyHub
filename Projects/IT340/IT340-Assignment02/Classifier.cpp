#include"Classifier.h"
Classifier::Classifier()
{
  this->tree=NULL;
}
Classifier::Classifier(istream&iStream,DecisionTree*tree,ostream&oStream)
{
  this->tree=tree;
  //Read the instances from the file
  this->readInsts(iStream);
  unsigned int i;
  //Decide for each instance
  for(i=0;i<this->getNumInsts();i++)
  {
    this->decide(this->getInst(i),this->tree->getRoot());
  }
  //Output to file
  this->output(oStream);
}
Classifier::Classifier(Classifier*orig)
{
  this->tree=orig->tree;
  this->instLst=orig->instLst;
}
Classifier Classifier::operator=(const Classifier&rhs)
{
  if(this==&rhs)
  {
    return*this;
  }
  this->tree=rhs.tree;
  this->instLst=rhs.instLst;
  return*this;
}
Classifier::~Classifier()
{
  //Nothing to do
}
void Classifier::addInst(shared_ptr<Instance>&inst)
{
  this->instLst.push_back(inst);
}
DecisionTree*Classifier::getTree()
{
  return this->tree;
}
shared_ptr<Instance>&Classifier::getInst(unsigned int index)
{
  return this->instLst[index];
}
unsigned int Classifier::getNumInsts()
{
  return this->instLst.size();
}
void Classifier::readInsts(istream&iStream)
{
  string line;
  //Each line is an instance
  while(getline(iStream,line))
  {
    //Handle the difference between how Unix and Windows handle newlines.
    line=line.substr(0,line.find('\r'));
    shared_ptr<Instance>inst(new Instance());
    //Read each attribute value from the line
    this->readInstAttrs(inst,line);
    //The instance has no classification
    inst->setClass(-1);
    this->addInst(inst);
  }
}
void Classifier::readInstAttrs(shared_ptr<Instance>&inst,string&line)
{
  unsigned int i,j;
  //Read in each attribute
  for(i=0;i<this->tree->getNumAttrs()-1;i++)
  {
    //The comma delimits each attribute
    unsigned long commaPos=line.find(",");
    //Get the attribute value from the line
    string attrVal=line.substr(0,commaPos);
    //Find the attribute value in the list of attribute values
    for(j=0;j<tree->getAttr(i).getNumVals();j++)
    {
      if(attrVal==tree->getAttr(i).getVal(j))
      {
        inst->addAttrValIndx(j);
        break;
      }
    }
    //Clip the attribute value from the line so we don't read it twice
    line=line.substr(commaPos+1);
  }
}
void Classifier::decide(shared_ptr<Instance>&inst,NodePtr&node)
{
  //Get the node's classification
  int nodeClass=node->getClass();
  //If the classification resolves the instance, set the classificaiton
  if(nodeClass!=-1)
  {
    inst->setClass(nodeClass);
    return;
  }
  else
  {
    //Otherwise get the attribute index that the node was split upon
    int splitAttr=node->getSplitAttr();
    //Get the instance's value for that attribute
    int instAttrVal=inst->getAttrValIndx(splitAttr);
    //Decide based on that splitting attribute
    this->decide(inst,node->getDecision(instAttrVal));
  }
}
void Classifier::output(ostream&oStream)
{
  unsigned int i,j;
  //Output each instance
  for(i=0;i<this->getNumInsts();i++)
  {
    //Output each attribute value
    for(j=0;j<this->tree->getNumAttrs()-1;j++)
    {
      int attrValIndx = this->getInst(i)->getAttrValIndx(j);
      oStream<<this->tree->getAttr(j).getVal(attrValIndx)<<",";
    }
    oStream<<this->tree->getClass(this->getInst(i)->getClass())<<endl;
  }
}
