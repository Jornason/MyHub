/* Decision Tree Printer Class
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 5:28am
 *
 * This class holds the methods used to print out a decision tree. */
#ifndef PRINTER_H_
#define PRINTER_H_
#include"DecisionTree.h"
#include<ostream>
using namespace std;
class Printer
{
  public:
  //Default constructor
    Printer();
  //Tree and output stream constructor
    Printer(DecisionTree*tree,ostream&oStream);
  //Copy constructor
    Printer(Printer*orig);
  //Overloaded assignment operator
    Printer operator=(const Printer&rhs);
    ~Printer();
  private:
  //Print a node (called recursively)
    void printNode(NodePtr&node,unsigned int indent,ostream&oStream);
  private:
  //The decision tree to print
    DecisionTree*tree;
};
#endif
