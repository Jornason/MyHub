//Attribute Class
//Author: Thomas Lyons
//Completed March 6, 2014 at 5:11am

//This class holds one attribute used to store information about an example or
//an instance for a naive bayes classifier */
#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_
#include<string>
#include<vector>
#include<memory>
using namespace std;
class Attribute
{
  public:
    //Default constructor
    Attribute();
    //Copy constructor
    Attribute(Attribute* orig);
    //Overloaded assignment operator
    Attribute operator=(const Attribute& rhs);
    //Destructor
    ~Attribute();
  public:
    //Setters
    void setName(string& name);
    void addVal(string& Val);
  public:
    //Getters
    string& getName();
    string& getVal(unsigned int index);
    unsigned int getNumVals();
  private:
    //The name of the attribute
    string name;
    //The possible values for the attribute
    vector<shared_ptr<string>>valLst;
};
#endif
