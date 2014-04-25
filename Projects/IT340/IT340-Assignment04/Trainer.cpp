#include"Trainer.h"

Trainer::Trainer()
{
  this->table = NULL;
}

Trainer::Trainer(istream& metaStream, istream& trainStream,
                 ProbabilityTable* table)
{
  this->table = table;
  /* Read the metadata */
  this->readMeta(metaStream);
  /* Read the training data */
  this->readTrain(trainStream);
  this->count();
}

Trainer::Trainer(Trainer* orig)
{
  this->table = orig->table;
  this->exampleList = orig->exampleList;
}

Trainer Trainer::operator=(const Trainer& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  this->table = rhs.table;
  this->exampleList = rhs.exampleList;
  return*this;
}

Trainer::~Trainer()
{
  //Nothing to do
}


void Trainer::addEx(shared_ptr<Example>& ex)
{
  this->exampleList.push_back(ex);
}

ProbabilityTable* Trainer::getTable()
{
  return this->table;
}

Example& Trainer::getEx(unsigned int index)
{
  return*this->exampleList[index];
}

unsigned int Trainer::getNumExs()
{
  return this->exampleList.size();
}

void Trainer::readMeta(istream& metaStream)
{
  string line;
  /* Each line is an attribute */
  while (getline(metaStream, line))
  {
    /* Handle the difference between how Unix and Windows handle newlines. */
    line = line.substr(0, line.find('\r'));
    while (!line.empty())
    {
      /* The attribute name is split from the values by a colon */
      unsigned long cutOff = line.find(":");
      string name = line.substr(0, cutOff);
      shared_ptr<Attribute>attr(new Attribute());
      /* Set the attribute's name */
      attr->setName(name);
      /* Cut off the name to avoid reading it twice */
      line = line.substr(cutOff + 1);
      while (!line.empty())
      {
        /* Commas delimit the attribute values */
        cutOff = line.find(",");
        if (cutOff != string::npos)
        {
          string val = line.substr(0, cutOff);
          attr->addVal(val);
          line = line.substr(cutOff + 1);
        }
        else
        {
          attr->addVal(line);
          line = "";
        }
      }
      this->table->addAttribute(attr);
    }
  }
}

void Trainer::readTrain(istream& trainStream)
{
  string line;
  unsigned int i;
  /* Each line is an example */
  while (getline(trainStream, line))
  {
    /* Handle the difference between how Unix and Windows handle newlines */
    line = line.substr(0, line.find('\r'));
    shared_ptr<Example>ex(new Example());
    /* Read the example attributes */
    this->readExAttrs(ex, line);
    /* Find the example's classification */
    for (i = 0; i < this->table->getNumberOfClassifications(); i++)
    {
      if (table->getClassification(i) == line)
      {
        ex->setClass(i);
        break;
      }
    }
    this->addEx(ex);
  }
}

void Trainer::readExAttrs(shared_ptr<Example>& ex, string& line)
{
  unsigned int i, j;
  /* Read in each attribute */
  for (i = 0; i < this->table->getNumberOfAttributes() - 1; i++)
  {
    /* Commas delimit each attribute */
    unsigned long commaPos = line.find(",");
    /* Get the attribute value from the line */
    string attrVal = line.substr(0, commaPos);
    /* Find the attribute value in the list of attribute values */
    for (j = 0; j < table->getAttribute(i).getNumVals(); j++)
    {
      if (attrVal == table->getAttribute(i).getVal(j))
      {
        ex->addAttrValIndx(j);
        break;
      }
    }
    /* Clip the attribute vale from the line so we don't read it twice */
    line = line.substr(commaPos + 1);
  }
}

void Trainer::count()
{
  unsigned int i, j, k;
  vector<int>classificationCount;
  //Initialize the classification counts to zero for each classification
  for (i = 0; i < this->table->getNumberOfClassifications(); i++)
  {
    classificationCount.push_back(0);
  }
  vector<vector<vector<int>>>attributeCounts;
  for (i = 0; i < this->table->getNumberOfAttributes() - 1; i++)
  {
    vector<vector<int>>attributeCount;
    for (j = 0; j < this->table->getAttribute(i).getNumVals(); j++)
    {
      //For each attribute, smooth the data by initializing to 1
      vector<int>valCount;
      for (k = 0; k < this->table->getNumberOfClassifications(); k++)
      {
        valCount.push_back(1);
      }
      attributeCount.push_back(valCount);
    }
    attributeCounts.push_back(attributeCount);
  }
  //Go through each example
  for (i = 0; i < this->exampleList.size(); i++)
  {
    Example ex = *exampleList[i];
    int classification = ex.getClass();
    //count the attributes
    for (j = 0; j < this->table->getNumberOfAttributes() - 1; j++)
    {
      attributeCounts[j][ex.getAttrValIndx(j)][classification]++;
    }
    //count the classification
    classificationCount[classification]++;
  }
  for (i = 0; i < this->table->getNumberOfAttributes() - 1; i++)
  {
    for (j = 0; j < this->table->getAttribute(i).getNumVals(); j++)
    {
      for (k = 0; k < this->table->getNumberOfClassifications(); k++)
      {
        //Find the probabilities
        //Probability =
        //number of times an attribute value occurred given a classification
        //    divided by
        //(number of times a classification occurred regardless of condition
        //    plus
        //the number of possible values for the attribute (to offset data
        //    smoothing))
        float probability = (float)attributeCounts[i][j][k] /
                            ((float)classificationCount[k] +
                             (float)this->table->getAttribute(i).getNumVals());
        this->table->setAttrProb(i, j, probability);
      }
    }
  }
  for (i = 0; i < this->table->getNumberOfClassifications(); i++)
  {
    //Find the probability fo a classification occurring regardless of condition
    //Probability =
    //Number of times classificaiton occurred plus 1 (data smoothing)
    //    divided by
    //Number of examples plus number of classifications (offset data smoothing)
    float probability = ((float)classificationCount[i] + 1) /
                        (this->exampleList.size() +
                         this->table->getNumberOfClassifications());
    this->table->setClassificationProbability(probability);
  }
}


