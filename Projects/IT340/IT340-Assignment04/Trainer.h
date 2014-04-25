//Naive Bayes Classifier Trainer Class
//Author: Thomas Lyons
//Completed April 15, 2014 at 2:21am

//This class holds the methods to construct a probablity table based on a series
//of examples.
#ifndef TRAINER_H_
#define TRAINER_H_
#include"ProbabilityTable.h"
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
    //Input stream and table constructor
    Trainer(istream& metaStream, istream& trainStream, ProbabilityTable* table);
    //Copy Constructor
    Trainer(Trainer* orig);
    //Overriden assignment operator
    Trainer operator=(const Trainer& rhs);
    //Destructor
    ~Trainer();
  private:
    // Setters (private)
    void addEx(shared_ptr<Example>& ex);
  private:
    //Getters (private)
    ProbabilityTable* getTable();
    Example& getEx(unsigned int index);
    unsigned int getNumExs();
  private:
    //Read a meta data file
    void readMeta(istream& metaStream);
    //Read a series of examples
    void readTrain(istream& trainStream);
    //Read each attribute value for the examples
    void readExAttrs(shared_ptr<Example>& ex, string& line);
    //Count the numbers of occurences of attributes and classifcations
    void count();
  private:
    //The probability table being trained
    ProbabilityTable* table;
    //The list of examples used to train
    ExLst exampleList;
};
#endif
