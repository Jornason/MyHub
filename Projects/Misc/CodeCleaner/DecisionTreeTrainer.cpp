#include "DecisionTreeTrainer.h"
#include <iostream>
using namespace std;
DecisionTreeTrainer::DecisionTreeTrainer() {
  this->tree = NULL; }
DecisionTreeTrainer::DecisionTreeTrainer(istream& metaDataStream,istream& trainingDataStream, DecisionTree* tree) {
  this->tree = tree;
  this->readInMetaData(metaDataStream);
  this->readInTrainingData(trainingDataStream);
  vector<int> openAttributeList;
  int i;
  for (i = 0; i < this->tree->getNumberOfAttributes() - 1; i++)openAttributeList.push_back(i);
  vector<shared_ptr<Example>> openExampleList;
  for (i = 0; i < this->getNumberOfExamples(); i++)openExampleList.push_back(this->exampleList.at(i));
  this->trainNode(tree->getRoot(), openExampleList, openAttributeList); }
DecisionTreeTrainer::DecisionTreeTrainer(DecisionTreeTrainer* original) {
  this->tree = original->tree;
  this->exampleList = original->exampleList; }
DecisionTreeTrainer DecisionTreeTrainer::operator =(
  const DecisionTreeTrainer& rhs) {
  if (this == &rhs) return *this;
  this->tree = rhs.tree;
  this->exampleList = rhs.exampleList;
  return *this; }
DecisionTreeTrainer::~DecisionTreeTrainer() {
  /* Nothing to do */
}
void DecisionTreeTrainer::addExample(Example& example) {
  this->exampleList.push_back(make_shared<Example>(example)); }
DecisionTree* DecisionTreeTrainer::getTree() {
  return this->tree; }
Example& DecisionTreeTrainer::getExample(int index) {
  return *this->exampleList[index]; }
int DecisionTreeTrainer::getNumberOfExamples() {
  return this->exampleList.size(); }
void DecisionTreeTrainer::readInMetaData(istream& metaDataStream) {
  string line;
  while (getline(metaDataStream, line)) {
    line = line.substr(0, line.find('\r'));
    while (!line.empty()) {
      unsigned long cutOffPosition = line.find(":");
      string attributeName = line.substr(0, cutOffPosition);
      Attribute attribute = new Attribute();
      attribute.setAttributeName(attributeName);
      line = line.substr(cutOffPosition + 1);
      while (!line.empty()) {
        cutOffPosition = line.find(",");
        if (cutOffPosition != string::npos) {
          string possibleValue = line.substr(0, cutOffPosition);
          attribute.addPossibleValue(possibleValue);
          line = line.substr(cutOffPosition + 1); }
        else {
          attribute.addPossibleValue(line);
          line = ""; } }
      this->tree->addAttribute(attribute); } } }
void DecisionTreeTrainer::readInTrainingData(istream& trainingDataStream) {
  string line;
  while (getline(trainingDataStream, line)) {
    line = line.substr(0, line.find('\r'));
    Example example = new Example();
    this->readExampleAttributes(example, line);
    for (int i = 0; i < this->tree->getNumberOfClassifications(); i++) {
      if (tree->getClassification(i) == line) {
        example.setClassificationIndex(i);
        break; } }
    this->addExample(example); } }
Example& DecisionTreeTrainer::readExampleAttributes(Example& example,
    string& line) {
  for (int i = 0; i < this->tree->getNumberOfAttributes() - 1; i++) {
    unsigned long commaPosition = line.find(",");
    string attributeValue = line.substr(0, commaPosition);
    for (int j = 0; j < tree->getAttribute(i).getNumberOfPossibleValues(); j++) {
      if (attributeValue == tree->getAttribute(i).getPossibleValue(j)) {
        example.addAttributeValueIndex(j);
        break; } }
    line = line.substr(commaPosition + 1); }
  return example; }
void DecisionTreeTrainer::trainNode(shared_ptr<DecisionTreeNode>& node,vector<shared_ptr<Example>>& openExampleList,vector<int>& openAttributeList) {
  double entropy = 0.0;
  int i;
  for (i = 0; i < this->tree->getNumberOfClassifications(); i++)entropy += calculateEntropy(i, openExampleList);
  double highestInformationGain = 0.0;
  int highestInformationGainIndex = 0;
  unsigned int j;
  for (j = 0; j < openAttributeList.size(); j++) {
    double informationGain = this->calculateInformationGain(entropy, openExampleList,openAttributeList.at(j));
    if (informationGain > highestInformationGain) {
      highestInformationGain = informationGain;
      highestInformationGainIndex = openAttributeList.at(j); } }
  (*node).setSplittingAttributeIndex(highestInformationGainIndex);
  for (i = 0;i <this->tree->getAttribute(highestInformationGainIndex).getNumberOfPossibleValues();i++) {
    shared_ptr<DecisionTreeNode> newNode (new DecisionTreeNode());
    vector<shared_ptr<Example>> newOpenExampleList;
    unsigned int k;
    for (k = 0; k < openExampleList.size(); k++) {
      if ((*openExampleList.at(k)).getAttributeValueIndex(highestInformationGainIndex)== i)newOpenExampleList.push_back(openExampleList.at(k)); }
    if(newOpenExampleList.size() == 0){
      (*node).addClassificationIndex(-2);
      (*node).addDecision(*newNode);
    }else{
      int classificationForNode = (*newOpenExampleList.at(0)).getClassificationIndex();
      for (k = 1; k < newOpenExampleList.size(); k++) {
        if (classificationForNode != (*newOpenExampleList.at(k)).getClassificationIndex()) {
          classificationForNode = -1;
          break; } }
      (*node).addClassificationIndex(classificationForNode);
      vector<int> newOpenAttributeList;
      for (k = 0; k < openAttributeList.size(); k++) {
        if (openAttributeList.at(k) != highestInformationGainIndex)
          newOpenAttributeList.push_back(openAttributeList.at(k)); }
      (*node).addDecision(*newNode);
      if ((*node).getClassificationIndex(i) == -1)
        this->trainNode((*node).getDecision(i), newOpenExampleList, newOpenAttributeList);
    }
  }
}
double DecisionTreeTrainer::calculateEntropy(int classificationIndex,vector<shared_ptr<Example>>& openExampleList) {
  int numberOfOccurrences = 0;
  for (unsigned int i = 0; i < openExampleList.size(); i++) {
    if ((*openExampleList.at(i)).getClassificationIndex() == classificationIndex)numberOfOccurrences++; }
  return -(((double)numberOfOccurrences / (double)openExampleList.size()) * this->log2((double)numberOfOccurrences /(double)openExampleList.size()));
}
double DecisionTreeTrainer::calculateInformationGain(double entropy,vector<shared_ptr<Example>>& openExampleList, int attribute) {
  int i;
  double sum = 0.0;
  for (i = 0; i < this->tree->getAttribute(attribute).getNumberOfPossibleValues(); i++) {
    unsigned int j;
    vector<shared_ptr<Example>> examplesWhereAttributeValueOccurred;
    for (j = 0; j < openExampleList.size(); j++) {
      if (i == (*openExampleList.at(j)).getAttributeValueIndex(attribute))examplesWhereAttributeValueOccurred.push_back(openExampleList.at(j)); }
    if(examplesWhereAttributeValueOccurred.size() == 0)continue;
    double attributeValueProbability = (double)examplesWhereAttributeValueOccurred.size() /
                                       (double)openExampleList.size();
    vector<int> classifications;
    classifications.push_back((*examplesWhereAttributeValueOccurred.at(0)).getClassificationIndex());
    unsigned int l;
    for (l = 1; l < examplesWhereAttributeValueOccurred.size();l++)
    {
      bool found = false;
      for(unsigned int m = 0; m < classifications.size();m++){
        if(classifications[m] == (*examplesWhereAttributeValueOccurred.at(l)).getClassificationIndex()){
          found = true;
          break;
        }
      }
      if(found){
        continue;
      }
      classifications.push_back((*examplesWhereAttributeValueOccurred.at(l)).getClassificationIndex());
    }
    if(classifications.size() == 1){
      return 1.0;
    }
    double entropySum = 0.0;
    int k;
    for (k = 0; k < this->tree->getNumberOfClassifications(); k++)
      entropySum += calculateEntropy(k, examplesWhereAttributeValueOccurred);
    double subSum = attributeValueProbability * entropySum;
    sum += subSum; }
  return entropy - sum; }

double DecisionTreeTrainer::log2(double x) {
  if (x == 0.0)return 0.0;
  return log(x) / log(2); }


