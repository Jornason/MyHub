//Naive Bayes Classifier Class
//Author: Thomas Lyons
//Completed April 15, 2014 at 2:12am

//This class contains the methods used to classify an instance into a category
//by using a probability table as a guide.
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_
#include"ProbabilityTable.h"
#include"Example.h"
#include<ostream>
#include<istream>
using namespace std;
//Specify a special case of example for this class
typedef Example Instance;
class Classifier
{
  public:
    //Default constructor
    Classifier();
    //I/O stream and table constructor
    Classifier(istream& iStream, ProbabilityTable* table, ostream& oStream);
    //Copy constructor
    Classifier(Classifier* orig);
    //Overloaded assignment operator
    Classifier operator=(const Classifier& rhs);
    //Destructor
    ~Classifier();
  protected:
    //Setters (protected)
    void addInst(shared_ptr<Instance>& inst);
  protected:
    //Getters (protected)
    ProbabilityTable* getTable();
    shared_ptr<Instance>& getInst(unsigned int index);
    unsigned int getNumInsts();
  protected:
    //Read in instances from the input stream
    void readInsts(istream& iStream);
    //Read the attribute values for the instance from a string
    void readInstAttrs(shared_ptr<Instance>& inst, string& line);
    //Classify the instance (called recursively)
    void decide(shared_ptr<Instance>& inst);
    //Output the result
    void output(ostream& oStream);
  protected:
    //The probability table
    ProbabilityTable* table;
    //The list of instances
    vector<shared_ptr<Instance>>instLst;
};
#endif
