#include"Classifier.h"

Classifier::Classifier()
{
  this->table = NULL;
}

Classifier::Classifier(istream& iStream, ProbabilityTable* table,
                       ostream& oStream)
{
  this->table = table;
  //Read the instances from the file
  this->readInsts(iStream);
  unsigned int i;
  //Make a decision for each instance
  for (i = 0; i < this->getNumInsts(); i++)
  {
    this->decide(getInst(i));
  }
  //Output to file
  this->output(oStream);
}

Classifier::Classifier(Classifier* orig)
{
  this->table = orig->table;
  this->instLst = orig->instLst;
}

Classifier Classifier::operator =(const Classifier& rhs)
{
  if (this == &rhs)
  {
    return*this;
  }
  this->table = rhs.table;
  this->instLst = rhs.instLst;
  return*this;
}

Classifier::~Classifier()
{
  //Nothing to do
}

void Classifier::addInst(shared_ptr<Instance>& inst)
{
  this->instLst.push_back(inst);
}

ProbabilityTable* Classifier::getTable()
{
  return this->table;
}

shared_ptr<Instance>& Classifier::getInst(unsigned int index)
{
  return this->instLst[index];
}

unsigned int Classifier::getNumInsts()
{
  return this->instLst.size();
}

void Classifier::readInsts(istream& iStream)
{
  string line;
  //Each line is an instance
  while (getline(iStream, line))
  {
    //Handle the difference between how Unix and Windows handle newlines.
    line = line.substr(0, line.find('\r'));
    shared_ptr<Instance>inst(new Instance());
    //Read each attribute value from the line
    this->readInstAttrs(inst, line);
    //The instance has no classification
    inst->setClass(-1);
    this->addInst(inst);
  }
}

void Classifier::readInstAttrs(shared_ptr<Instance>& inst, string& line)
{
  unsigned int i, j;
  //Read in each attribute
  for (i = 0; i < this->table->getNumberOfAttributes() - 1; i++)
  {
    //The comma delimits each attribute
    unsigned long commaPos = line.find(",");
    //Get the attribute value from the line
    string attrVal = line.substr(0, commaPos);
    //Find the attribute value in the list of attribute values
    for (j = 0; j < table->getAttribute(i).getNumVals(); j++)
    {
      if (attrVal == table->getAttribute(i).getVal(j))
      {
        inst->addAttrValIndx(j);
        break;
      }
    }
    //Clip the attribute value from the line so we don't read it twice
    line = line.substr(commaPos + 1);
  }
}

void Classifier::decide(shared_ptr<Instance>& inst)
{
  vector<double>classificationProbabilities;
  unsigned int i;
  //For each classification, find the probability that it will occur given the
  //attributes in the instance
  for (i = 0; i < table->getNumberOfClassifications(); i++)
  {
    //This is the likelyhood of a classification occurring regardless of
    //conditions
    double probability = table->getClassProbability(i);
    //Multiply that probablity by the probability of the value for each
    //attribute in this instance occurring given the classification
    for (unsigned int j = 0; j < table->getNumberOfAttributes() - 1; j++)
    {
      probability *= table->getAttrProb(j, inst->getAttrValIndx(j), i);
    }
    //Save the result
    classificationProbabilities.push_back(probability);
  }
  //Find the classification that yielded the highest probability
  unsigned int maxProbabilityIndex = -1;
  double maxProbability = -1.0;
  for (i = 0; i < classificationProbabilities.size(); i++)
  {
    if (classificationProbabilities[i] > maxProbability)
    {
      maxProbability = classificationProbabilities[i];
      maxProbabilityIndex = i;
    }
  }
  inst->setClass(maxProbabilityIndex);
}

void Classifier::output(ostream& oStream)
{
  unsigned int i, j;
  //Output each instance
  for (i = 0; i < this->getNumInsts(); i++)
  {
    //Output each attribute value
    for (j = 0; j < this->table->getNumberOfAttributes() - 1; j++)
    {
      int attrValIndx = this->getInst(i)->getAttrValIndx(j);
      oStream << this->table->getAttribute(j).getVal(attrValIndx) << ",";
    }
    oStream << this->table->getClassification(this->getInst(i)->getClass());
    oStream << endl;
  }
}
