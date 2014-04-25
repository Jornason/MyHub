#include "ProbabilityTable.h"

ProbabilityTable::ProbabilityTable()
{
  /* Nothing to do */
}

ProbabilityTable::ProbabilityTable(ProbabilityTable* orig)
{
  this->attributeProbabilities     = orig->attributeProbabilities;
  this->classificationProbabilities = orig->classificationProbabilities;
  this->attributeList              = orig->attributeList;
}

ProbabilityTable ProbabilityTable::operator=(const ProbabilityTable& rhs)
{
  if (this == &rhs)
  {
    return*this;
  }
  this->attributeProbabilities = rhs.attributeProbabilities;
  this->classificationProbabilities = rhs.classificationProbabilities;
  this->attributeList = rhs.attributeList;
  return*this;
}

ProbabilityTable::~ProbabilityTable()
{
  /* Nothing to do */
}

void ProbabilityTable::setAttrProb(int attribute, int valIndex,
                                   float probability)
{
  attributeProbabilities[attribute][valIndex].push_back(probability);
}

void ProbabilityTable::setClassificationProbability(float probability)
{
  classificationProbabilities.push_back(probability);
}

void ProbabilityTable::addAttribute(shared_ptr<Attribute>& attribute)
{
  unsigned int i;
  vector<vector<float>>attributeProbabilityVector;
  for (i = 0; i < (*attribute).getNumVals(); i++)
  {
    vector<float>attributeValProbabilities;
    attributeProbabilityVector.push_back(attributeValProbabilities);
  }
  attributeProbabilities.push_back(attributeProbabilityVector);
  attributeList.push_back(attribute);
}

float ProbabilityTable::getAttrProb(int attribute, int valIndex, int c)
{
  return attributeProbabilities[attribute][valIndex][c];
}

float ProbabilityTable::getClassProbability(int classification)
{
  return classificationProbabilities[classification];
}

Attribute& ProbabilityTable::getAttribute(int index)
{
  return*this->attributeList[index];
}

string& ProbabilityTable::getClassification(int index)
{
  return this->attributeList[this->attributeList.size() - 1]->getVal(index);
}

unsigned int ProbabilityTable::getNumberOfAttributes()
{
  return this->attributeList.size();
}

unsigned int ProbabilityTable::getNumberOfClassifications()
{
  return this->attributeList[this->attributeList.size() - 1]->getNumVals();
}

