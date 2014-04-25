#include"Trainer.h"
using namespace std;
Trainer::Trainer()
{
  this->tree=NULL;
}
Trainer::Trainer(istream&metaStream,istream&trainStream,DecisionTree*tree)
{
  this->tree=tree;
  //Read the meta data
  this->readMeta(metaStream);
  //Read the training data
  this->readTrain(trainStream);
  vector<int>attrLst;
  unsigned int i;
  //All attributes are open for consideration
  for(i=0;i<this->tree->getNumAttrs()-1;i++)
  {
    attrLst.push_back(i);
  }
  ExLst exLst;
  //All examples are open for consideration
  for(i=0;i<this->getNumExs();i++)
  {
    exLst.push_back(this->exLst.at(i));
  }
  //Train the root
  this->train(tree->getRoot(),exLst,attrLst);
}
Trainer::Trainer(Trainer*orig)
{
  this->tree=orig->tree;
  this->exLst=orig->exLst;
}
Trainer Trainer::operator=(const Trainer&rhs)
{
  if(this==&rhs)
  {
    return *this;
  }
  this->tree=rhs.tree;
  this->exLst=rhs.exLst;
  return*this;
}
Trainer::~Trainer()
{
  //Nothing to do
}
void Trainer::addEx(shared_ptr<Example>&ex)
{
  this->exLst.push_back(ex);
}
DecisionTree*Trainer::getTree()
{
  return this->tree;
}
Example&Trainer::getEx(unsigned int index)
{
  return*this->exLst[index];
}
unsigned int Trainer::getNumExs()
{
  return this->exLst.size();
}
void Trainer::readMeta(istream&metaStream)
{
  string line;
  //Each line is an attribute
  while(getline(metaStream,line))
  {
    //Handle the difference between how Unix and Windows handle newlines.
    line=line.substr(0,line.find('\r'));
    while(!line.empty())
    {
      //The attribute name is split from the values by a colon
      unsigned long cutOff=line.find(":");
      string name=line.substr(0,cutOff);
      shared_ptr<Attribute>attr(new Attribute());
      //Set the attribute's name
      attr->setName(name);
      //Cut off the name to avoid reading it twice
      line=line.substr(cutOff+1);
      while(!line.empty())
      {
        //Commas delimit the attribute values
        cutOff=line.find(",");
        if(cutOff!=string::npos)
        {
          string val=line.substr(0,cutOff);
          attr->addVal(val);
          line=line.substr(cutOff+1);
        }
        else
        {
          attr->addVal(line);
          line="";
        }
      }
      this->tree->addAttr(attr);
    }
  }
}
void Trainer::readTrain(istream&trainStream)
{
  string line;
  unsigned int i;
  //Each line is an example
  while(getline(trainStream,line))
  {
    //Handle the difference between how Unix and Windows handle newlines.
    line=line.substr(0,line.find('\r'));
    shared_ptr<Example>ex(new Example());
    //Read the example attributes
    this->readExAttrs(ex,line);
    //Find the example's classification
    for(i=0;i<this->tree->getNumClasses();i++)
    {
      if(tree->getClass(i)==line)
      {
        ex->setClass(i);
        break;
      }
    }
    this->addEx(ex);
  }
}
void Trainer::readExAttrs(shared_ptr<Example>&ex,string&line)
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
        ex->addAttrValIndx(j);
        break;
      }
    }
    //Clip the attribute value from the line so we don't read it twice
    line=line.substr(commaPos+1);
  }
}
void Trainer::train(NodePtr&node,ExLst&exLst,IndxLst&attrLst)
{
  unsigned int i,j;
  /* If there are no more attributes to consider, find the most common
   * classification among the examples to consider. */
  if(attrLst.size()==0)
  {
    node->setClass(mostCommonClass(exLst));
    return;
  }
  //Get the attribute that yields the most information
  int splitAttr=bestAttr(exLst,attrLst);
  node->setSplitAttr(splitAttr);
  unsigned numAttrVals=this->tree->getAttr(splitAttr).getNumVals();
  //Create a sub-node for each possible value of the attribute
  for(i=0;i<numAttrVals;i++)
  {
    NodePtr newNode(new Node());
    ExLst newExLst;
    //Find the examples that are relevant to this attribute
    for(j=0;j<exLst.size();j++)
    {
      if(exLst[j].get()->getAttrValIndx(splitAttr)==i)
      {
        newExLst.push_back(exLst[j]);
      }
    }
    /* If there are no more examples to consider, find the most common
     * classification among the old example list. */
    if(newExLst.size()==0)
    {
      /* Set the node's classification, add a decision, and continue to the next
       * attribute. */
      newNode->setClass(mostCommonClass(exLst));
      node->addDecision(newNode);
      continue;
    }
    //Find if the new example list has only one classification
    int nodeClass=newExLst[0].get()->getClass();
    for(j=0;j<newExLst.size();j++)
    {
      int newClass=newExLst[j].get()->getClass();
      if(nodeClass!=newClass)
      {
        nodeClass=-1;
        break;
      }
    }
    /* If the node is conclusive, set the nodes's class, add a decision, and
     * continue to the next attribute. */
    if(nodeClass!=-1)
    {
      newNode->setClass(nodeClass);
      node->addDecision(newNode);
      continue;
    }
    IndxLst newAttrLst;
    /* Remove the attribute we just split upon from the list of attributes under
     * consideration. */
    for(j=0;j<attrLst.size();j++)
    {
      if(attrLst[j]!=splitAttr)
      {
        newAttrLst.push_back(attrLst[j]);
      }
    }
    //Add a decision and train based upon the sub-node
    node->addDecision(newNode);
    this->train(newNode,newExLst,newAttrLst);
  }
}
unsigned int Trainer::bestAttr(ExLst&exLst,IndxLst&attrLst)
{
  unsigned int numClasses=this->tree->getNumClasses();
  unsigned int numAttrs=attrLst.size();
  double totalEntropy=0.0;
  unsigned int i;
  //Find the total entropy among all of the examples for all classifications
  for(i=0;i<numClasses;i++)
  {
    totalEntropy+=calcEntropy(i,exLst);
  }
  int bestInfoGainIndx=attrLst[0];
  double bestInfoGain=calcInfoGain(totalEntropy,exLst,attrLst[0]);
  //Find the attribute with the most information to yield
  for(i=0;i<numAttrs;i++)
  {
    double newInfoGain=calcInfoGain(totalEntropy,exLst,attrLst[i]);
    if(bestInfoGain<newInfoGain)
    {
      bestInfoGainIndx=attrLst[i];
      bestInfoGain=newInfoGain;
    }
  }
  return bestInfoGainIndx;
}
double Trainer::calcEntropy(int classIndx,ExLst&exLst)
{
  //Find the probability of a classification occurring in an example list
  int numTimes=0;
  for(unsigned int i=0;i<exLst.size();i++)
  {
    if(exLst.at(i)->getClass()==classIndx)
    {
      numTimes++;
    }
  }
  double prob=(double)numTimes/(double)exLst.size();
  //Use the super-secret entropy formula
  return-(prob*this->log2(prob));
}
double Trainer::calcInfoGain(double entropy,ExLst&exLst,int attr)
{
  unsigned int i,j,k;
  double sum=0.0;
  //Consider all attribute values
  for(i=0;i<this->tree->getAttr(attr).getNumVals();i++)
  {
    ExLst exsWithAttrVal;
    //For each attribute value, find the relative examples
    for(j=0;j<exLst.size();j++)
    {
      if(i==exLst.at(j)->getAttrValIndx(attr))
      {
        exsWithAttrVal.push_back(exLst.at(j));
      }
    }
    //If there are no examples with the value, continue to the next value
    if(exsWithAttrVal.size()==0)
    {
      continue;
    }
    //Find the probability an example having that value
    double attrValProb=(double)exsWithAttrVal.size()/(double)exLst.size();
    IndxLst classes;
    classes.push_back(exsWithAttrVal.at(0)->getClass());
    //Get all of the classifications among the examples
    for(j=1;j<exsWithAttrVal.size();j++)
    {
      bool found=false;
      for(k=0;k<classes.size();k++)
      {
        if(classes[k]==exsWithAttrVal.at(j)->getClass())
        {
          found=true;
          break;
        }
      }
      if(found)
      {
        continue;
      }
      classes.push_back(exsWithAttrVal.at(j)->getClass());
    }
    //If there is only one classification, there is perfect information gain
    if(classes.size()==1)
    {
      return 1.0;
    }
    //Find the sum of the entropy across all of the classifications
    double entropySum=0.0;
    for(j=0;j<this->tree->getNumClasses();j++)
    {
      entropySum+=calcEntropy(j,exsWithAttrVal);
    }
    //Multiply by the probability of the attribute having that value
    double subSum=attrValProb*entropySum;
    sum+=subSum;
  }
  //Return the information gain
  return entropy-sum;
}
double Trainer::log2(double x)
{
  //This log2 implementation won't allow an undefined result for zero
  if(x==0.0)
  {
    return 0.0;
  }
  return log(x)/log(2);
}
int Trainer::mostCommonClass(ExLst&exLst)
{
  unsigned int i,j;
  int maxNumTimes=0;
  int mostCommonClass=exLst[0].get()->getClass();
  //Find the most common classification among the examples
  for(i=0;i<this->tree->getNumClasses();i++)
  {
    int numTimes=0;
    for(j=0;j<exLst.size();j++)
    {
      if(exLst[j].get()->getClass()==i)
      {
        numTimes++;
      }
    }
    if(numTimes>maxNumTimes)
    {
      maxNumTimes=numTimes;
      mostCommonClass=i;
    }
  }
  return mostCommonClass;
}
