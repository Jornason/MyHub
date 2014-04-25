#include"Report.h"

Report::Report(): Classifier()
{
  //Nothing else to do
}

Report::Report(istream& iStream, ProbabilityTable* table, ostream& oStream)
{
  this->table = table;
  //Read the instances from the file
  this->readInsts(iStream);
  unsigned int i;
  //Decide for each instance
  for (i = 0; i < this->getNumInsts(); i++)
  {
    this->decide(this->getInst(i));
  }
  //Output to file
  this->output(oStream);
}

Report::Report(Report* orig): Classifier(orig)
{
  this->passFail = orig->passFail;
}

Report Report::operator=(const Report& rhs)
{
  if (this == &rhs)
  {
    return*this;
  }
  this->table = rhs.table;
  this->instLst = rhs.instLst;
  this->passFail = rhs.passFail;
  return*this;
}

Report::~Report()
{
  //Nothing to do
}

void Report::readInsts(istream& iStream)
{
  unsigned int i;
  string line;
  //Each line is a test instance
  while (getline(iStream, line))
  {
    //Handle the difference between how Unix and Windows handle newlines.
    line = line.substr(0, line.find('\r'));
    shared_ptr<TestInstance>testInst(new TestInstance());
    //Read each attribute value from the line
    this->readInstAttrs(testInst, line);
    //Set the test instance's class
    for (i = 0; i < this->table->getNumberOfClassifications(); i++)
    {
      if (table->getClassification(i) == line)
      {
        testInst->setClass(i);
        break;
      }
    }
    this->addInst(testInst);
  }
}

void Report::decide(shared_ptr<TestInstance>& testInst)
{
  vector<double>classificationProbabilities;
  unsigned int i;
  //The first part is just lke the Classifer's decide
  for (i = 0; i < table->getNumberOfClassifications(); i++)
  {
    double probability = table->getClassProbability(i);
    for (unsigned int j = 0; j < table->getNumberOfAttributes() - 1; j++)
    {
      probability *= table->getAttrProb(j, testInst->getAttrValIndx(j), i);
    }
    classificationProbabilities.push_back(probability);
  }
  //Find the highest probability
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
  //If they match, it passes. Otherwise it fails
  if (maxProbabilityIndex == testInst->getClass())
  {
    passFail.push_back(true);
  }
  else
  {
    passFail.push_back(false);
  }
}


void Report::output(ostream& oStream)
{
  int numPass = 0;
  int numFail = 0;
  unsigned int i;
  //Count the number of passes and fails
  for (i = 0; i < passFail.size(); i++)
  {
    if (passFail[i])
    {
      numPass++;
    }
    else
    {
      numFail++;
    }
  }
  //Output the score
  double score = (double)numPass / (double)passFail.size();
  oStream << "Decision tree score: " << 100.0 * score << "%" << endl;
  oStream << "Passed: " << numPass << " Failed: " << numFail << endl;
  oStream << "Total: " << passFail.size() << endl;
}

