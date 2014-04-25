//Naive Bayes Classifier Probability Table Class
//Author: Thomas Lyons
//Completed April 15, 2014 at 2:16am

//This class represents a probability look-up table for the Naive Probability
//Classifier
#ifndef PROBABILITYTABLE_H
#define PROBABILITYTABLE_H
#include "Attribute.h"
using namespace std;
class ProbabilityTable
{
  public:
    //Default constructor
    ProbabilityTable();
    //Copy Constructor
    ProbabilityTable(ProbabilityTable* orig);
    //Overridden assignment operator
    ProbabilityTable operator=(const ProbabilityTable& rhs);
    //Destructor
    ~ProbabilityTable();
  public:
    //Setters
    void setAttrProb(int attribute, int valIndex, float probability);
    void setClassificationProbability(float probability);
    void addAttribute(shared_ptr<Attribute>& attribute);
  public:
    //Getters
    float        getAttrProb(int attribute, int valIndex, int classification);
    float        getClassProbability(int classification);
    Attribute&   getAttribute(int index);
    string&      getClassification(int index);
    unsigned int getNumberOfAttributes();
    unsigned int getNumberOfClassifications();
  private:
    //This table holds the attribute probabilities. It is the probability that
    //an attribte will be true given a classification
    vector<vector<vector<float>>> attributeProbabilities;
    //This holds the the classification probabilities. Is is the probability
    //that a classification will be true
    vector<float>                 classificationProbabilities;
    //The list of attributes. The last attribute is the classifications
    vector<shared_ptr<Attribute>> attributeList;
};

#endif
