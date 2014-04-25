/* Decision Tree Accuracy Report Class
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 5:34am
 *
 * This class holds the methods to report on the accuracy of a decision tree
 * based on a series of classified instances. It overrides many of Classifiers
 * methods. */
#ifndef REPORT_H
#define REPORT_H
#include"DecisionTree.h"
#include"Example.h"
#include"Classifier.h"
using namespace std;
//Handy type definition for test instances
typedef Example TestInstance;
class Report:public Classifier
{
  public:
  //Default constructor
    Report();
  //I/O stream and decision tree constructor
    Report(istream&iStream,DecisionTree*tree,ostream&oStream);
  //Copy constructor
    Report(Report*orig);
  //Overloaded assignment operator
    Report operator=(const Report&rhs);
  //Destructor
    ~Report();
  private:
    /* Override Classifier's instance reader to read instances WITH
     * classifications. */
    void readInsts(istream&iStream);
    //Override Classifier's decide method to report pass or fail
    void decide(shared_ptr<TestInstance>&testInst,NodePtr&node);
    //Override Classifiers output method to print a score
    void output(ostream&oStream);
  private:
    //The stored results of the tests
    vector<bool>passFail;
};
#endif
