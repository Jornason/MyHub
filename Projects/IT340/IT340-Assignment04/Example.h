//Example Class
//Author: Thomas Lyons
//Completed March 6, 2014 at 5:21am

//This class holds one example used to train a naive bayes classifier. It is
//also used to represent instances that have not yet been classified and test
//instances which are being used to judge the table. */
#ifndef EXAMPLE_H_
#define EXAMPLE_H_
#include<vector>
using namespace std;
//Handy type definition for index lists
typedef vector<int> IndxLst;
class Example
{
  public:
    //Default constructor
    Example();
    //Copy constructor
    Example(Example* orig);
    //Overloaded assignment operator
    Example operator=(const Example& rhs);
    //Destructor
    ~Example();
  public:
    //Setters
    void addAttrValIndx(int attrValIndx);
    void setClass(int classIndx);
  public:
    //Getters
    int getAttrValIndx(int index);
    int getClass();
  private:
    //The list of attribue value indices. These will be mapped to an attribute
    //value later
    IndxLst attrValIndxs;
    //The classification of this example. This will be mapped to a
    //classification later
    int classIndx;
};
#endif
