/* Decision Tree Trainer Class
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 5:39am
 *
 * This class holds the methods to constructor a decision tree based on a series
 * of examples. */
#ifndef TRAINER_H_
#define TRAINER_H_
#include"DecisionTree.h"
#include"Example.h"
#include<fstream>
#include<istream>
#include<math.h>
using namespace std;
//Handy type definition for a series of examples
typedef vector<shared_ptr<Example>> ExLst;
class Trainer
{
  public:
  //Default constructor
    Trainer();
  //Input Stream and tree constructor
    Trainer(istream&metaStream,istream&trainStream,DecisionTree*tree);
  //Copy constructor
    Trainer(Trainer*orig);
  //Overloaded assignment operator
    Trainer operator=(const Trainer&rhs);
  //Destructor
    ~Trainer();
  private:
  //Setters (private)
    void addEx(shared_ptr<Example>&ex);
  private:
  //Getters (private)
    DecisionTree*getTree();
    Example&getEx(unsigned int index);
    unsigned int getNumExs();
  private:
    //Read a meta data file
    void readMeta(istream&metaStream);
    //Read a series of examples
    void readTrain(istream&trainStream);
    //Read each attribute value for the examples
    void readExAttrs(shared_ptr<Example>&ex,string&line);
    //Train a node in the tree
    void train(NodePtr&node,ExLst&exLst,IndxLst&attrLst);
    //Find the attribute with the most information to yield
    unsigned int bestAttr(ExLst&exLst,IndxLst&attrLst);
    //Calculate the entropy of a list of examples based on a classification
    double calcEntropy(int classIndx,ExLst&exLst);
    //Calculate the information yielded from an attribute
    double calcInfoGain(double entropy,ExLst&exLst,int attr);
    //Log base 2 (Handles special log2(0) case)
    double log2(double x);
    //Find the most common classification among a list of examples
    int mostCommonClass(ExLst&exLst);
  private:
    //The decision tree
    DecisionTree*tree;
    //The list of examples
    ExLst exLst;
};
#endif
