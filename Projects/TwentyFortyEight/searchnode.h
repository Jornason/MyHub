#ifndef SEARCHNODE_H
#define SEARCHNODE_H
#include<memory>
#include<iostream>
#include"problemstate.h"
using namespace std;
class SearchNode
{
  public:
    SearchNode();
    SearchNode(ProblemState&prob);
    SearchNode(SearchNode*orig);
    ~SearchNode();
  public:
    SearchNode operator=(const SearchNode&rhs);
    bool operator==(const SearchNode&other)const;
  public:
    bool checkBacktrack();
    void printSolution();
  public:
    SearchNode& getParent();
    ProblemState& getState();
    void setParent(SearchNode&parent);
    void setState(ProblemState&state);
  private:
    bool checkAncestor(SearchNode*that);
  private:
    SearchNode* parent;
    ProblemState state;
};

#endif // SEARCHNODE_H
