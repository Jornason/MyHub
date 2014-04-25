#include"Report.h"
Report::Report():Classifier()
{
  //Nothing else to do
}
Report::Report(istream&iStream,DecisionTree*tree,ostream&oStream)
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
Report::Report(Report*orig):Classifier(orig)
{
  this->passFail=orig->passFail;
}
Report Report::operator=(const Report&rhs)
{
  if(this==&rhs)
  {
    return*this;
  }
  this->tree=rhs.tree;
  this->instLst=rhs.instLst;
  this->passFail=rhs.passFail;
  return*this;
}
Report::~Report()
{
  //Nothing to do
}
void Report::readInsts(istream&iStream)
{
  unsigned int i;
  string line;
  //Each line is a test instance
  while(getline(iStream,line))
  {
    //Handle the difference between how Unix and Windows handle newlines.
    line=line.substr(0,line.find('\r'));
    shared_ptr<TestInstance>testInst(new TestInstance());
    //Read each attribute value from the line
    this->readInstAttrs(testInst,line);
    //Set the test instance's class
    for(i=0;i<this->tree->getNumClasses();i++)
    {
      if(tree->getClass(i)==line)
      {
        testInst->setClass(i);
        break;
      }
    }
    this->addInst(testInst);
  }
}
void Report::decide(shared_ptr<TestInstance>&testInst,NodePtr&node)
{
  //Get the node's classification
  int nodeClass=node->getClass();
  //If the classification resolves the test instance, see if it passed or failed
  if(nodeClass!=-1)
  {
    if(testInst->getClass()==nodeClass)
    {
      passFail.push_back(true);
    }
    else
    {
      passFail.push_back(false);
    }
    return;
  }
  else
  {
    //Otherwise get the attribute index that the node was split upon
    int splitAttr=node->getSplitAttr();
    //Get the instance's value for that attribute
    int testInstAttrVal=testInst->getAttrValIndx(splitAttr);
    //Decide based upon that splitting attribute
    this->decide(testInst,node->getDecision(testInstAttrVal));
  }
}
void Report::output(ostream&oStream)
{
  int numPass=0;
  int numFail=0;
  unsigned int i;
  //Count the number of passes and fails
  for(i=0;i<passFail.size();i++)
  {
    if(passFail[i])
    {
      numPass++;
    }
    else
    {
      numFail++;
    }
  }
  //Output the score
  double score=(double)numPass/(double)passFail.size();
  oStream<<"Decision tree score: "<<100.0*score<<"%"<<endl;
  oStream<<"Passed: "<<numPass<<" Failed: "<<numFail<<endl;
  oStream<<"Total: "<<passFail.size()<<endl;
}

