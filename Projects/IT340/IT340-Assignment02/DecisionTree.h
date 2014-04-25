/* Decision Tree Class
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 5:18am
 *
 * This class holds a decision tree and it's attribute list. */
#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_
#include"Attribute.h"
#include"Node.h"
using namespace std;
//Handy type definition for a shared pointer to a node
typedef shared_ptr<Node> NodePtr;
class DecisionTree
{
  public:
  //Default constructor
    DecisionTree();
  //Copy constructor
    DecisionTree(DecisionTree*orig);
  //Overloaded assignment operator
    DecisionTree operator=(const DecisionTree&rhs);
  //Destructor
    ~DecisionTree();
  public:
  //Setter
    void addAttr(shared_ptr<Attribute>&attr);
  public:
  //Getters
    NodePtr&getRoot();
    Attribute&getAttr(unsigned int index);
    string&getClass(unsigned int index);
    unsigned int getNumAttrs();
    unsigned int getNumClasses();
  private:
    //The root of the tree
    NodePtr root;
    //The list of attributes. The last attribute is the classifications
    vector<shared_ptr<Attribute>>attrLst;
};
#endif
