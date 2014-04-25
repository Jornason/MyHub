//Naive Bayes Classifier Accuracy Report Class
//Author: Thomas Lyons
//Completed April 15, 2014 at 2:19am

//This class holds the methods to report on the accuracy of a naive bayes
//classifier based on a series of classified instances. It overrides many of
//Classifiers methods.
#ifndef REPORT_H
#define REPORT_H
#include"Example.h"
#include"Classifier.h"
using namespace std;
//Handy type definition for test instances
typedef Example TestInstance;
class Report: public Classifier
{
  public:
    //Default constructor
    Report();
    //I/O stream and probability table constructor
    Report(istream& iStream, ProbabilityTable* table, ostream& oStream);
    //Copy constructor
    Report(Report* orig);
    //Overloaded assignment operator
    Report operator=(const Report& rhs);
    //Destructor
    ~Report();
  private:
    //Override Classifier's instance reader to read instances WITH
    //classifications.
    void readInsts(istream& iStream);
    //Override Classifier's decide method to report pass or fail
    void decide(shared_ptr<TestInstance>& testInst);
    //Override Classifiers output method to print a score
    void output(ostream& oStream);
  private:
    //The stored results of the tests
    vector<bool>passFail;
};
#endif
