/* Decision Tree Classifier Class
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 5:14am
 *
 * This class contains the methods used to classify an instance into a category
 * by using a decision tree as a guide. */
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_
#include"DecisionTree.h"
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
  //I/O stream and tree constructor
    Classifier(istream&iStream,DecisionTree*tree,ostream&oStream);
  //Copy constructor
    Classifier(Classifier*orig);
  //Overloaded assignment operator
    Classifier operator=(const Classifier&rhs);
  //Destructor
    ~Classifier();
  protected:
  //Setters (protected)
    void addInst(shared_ptr<Instance>&inst);
  protected:
  //Getters (protected)
    DecisionTree*getTree();
    shared_ptr<Instance>&getInst(unsigned int index);
    unsigned int getNumInsts();
  protected:
    //Read in instances from the input stream
    void readInsts(istream&iStream);
    //Read the attribute values for the instance from a string
    void readInstAttrs(shared_ptr<Instance>&inst,string&line);
    //Classify the instance (called recursively)
    void decide(shared_ptr<Instance>&inst,NodePtr&node);
    //Output the result
    void output(ostream&oStream);
  protected:
    //The decision tree
    DecisionTree*tree;
    //The list of instances
    vector<shared_ptr<Instance>>instLst;
};
#endif
