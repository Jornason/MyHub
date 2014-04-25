/* Node Class
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 5:25am
 *
 * This class uses holds one decision tree node. It stores a list of sub-nodes,
 * a classification index, and an attribute index upon which the node was split.
 * */
#ifndef NODE_H_
#define NODE_H_
#include<memory>
#include<vector>
using namespace std;
class Node
{
  public:
  //Default constructor
    Node();
  //Copy constructor
    Node(Node*orig);
  //Overloaded assignment operator
    Node operator=(const Node&rhs);
  //Destructor
    ~Node();
  public:
  //Setters
    void addDecision(shared_ptr<Node>&node);
    void setClass(int classIndx);
    void setSplitAttr(int splitAttr);
  public:
  //Getters
    shared_ptr<Node>&getDecision(int index);
    int getClass();
    int getSplitAttr();
    unsigned int getNumDecisions();
  private:
    //The sub-nodes
    vector<shared_ptr<Node>>decisionLst;
    //The classification of this node (-1 = inconclusive)
    int classIndx;
    //The splitting attribute index upon which the node was split
    int splitAttr;
};
#endif
