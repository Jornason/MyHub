#include"Printer.h"

Printer::Printer()
{
  this->table = NULL;
}

Printer::Printer(ProbabilityTable* table, ostream& oStream)
{
  //Set the precision of the data to print
  oStream.precision(8);
  //Fixed decimal format
  oStream << fixed;
  this->table = table;
  unsigned int i, j, k;
  oStream << "           ";
  //Print each classification name (maximum 10 characters)
  for (i = 0; i < this->table->getNumberOfClassifications(); i++)
  {
    oStream << setw(10) << this->table->getClassification(i) << setw(1);
    oStream << " ";
  }
  oStream << endl << "           ";
  //Print the probabilities that the classificaiton will occur regardless of
  //conditions
  for (i = 0; i < this->table->getNumberOfClassifications(); i++)
  {
    oStream << setw(10) << this->table->getClassProbability(i) << setw(1);
    oStream << " ";
  }
  oStream << endl << endl;
  for (i = 0; i < this->table->getNumberOfAttributes() - 1; i++)
  {
    for (j = 0; j < this->table->getAttribute(i).getNumVals(); j++)
    {
      //Print each possible attribute value
      oStream << setw(10) << this->table->getAttribute(i).getVal(j) << setw(1);
      oStream << " ";
      for (k = 0; k < this->table->getNumberOfClassifications(); k++)
      {
        //Print the probability of that attribute value occurring given the
        //classification
        oStream << setw(10) << this->table->getAttrProb(i, j, k) << setw(1);
        oStream << " ";
      }
      oStream << endl;
    }
    oStream << endl;
  }
}

Printer::Printer(Printer* orig)
{
  this->table = orig->table;
}

Printer Printer::operator=(const Printer& rhs)
{
  if (this == &rhs)
  {
    return*this;
  }
  this->table = rhs.table;
  return*this;
}

Printer::~Printer()
{
  /* Nothing to do */
}
