//Naive Bayes Classifier Printer Class
//Author: Thomas Lyons
//Completed April 15, 2014 at 2:14am

//This class holds the methods used to print out a probability table
#ifndef PRINTER_H_
#define PRINTER_H_
#include"ProbabilityTable.h"
#include<ostream>
#include<iomanip>
using namespace std;
class Printer
{
  public:
    //Default Constructor
    Printer();
    //Table and Output stream Constructor
    Printer(ProbabilityTable* table, ostream& oStream);
    //Copy Constructor
    Printer(Printer* orig);
    //Overridden Assignment Operator
    Printer operator=(const Printer& rhs);
    //Destructor
    ~Printer();
  private:
    //Probability Table
    ProbabilityTable* table;
};
#endif
