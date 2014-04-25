#ifndef PROBLEMSTATE_H
#define PROBLEMSTATE_H
#include<vector>
#include<memory>
#include<stdio.h>
using namespace std;
class ProblemState
{
  public:
    ProblemState();
    ProblemState(ProblemState*orig);
    ~ProblemState();
  public:
    ProblemState operator=(const ProblemState&rhs);
    bool operator==(const ProblemState&other) const;
    bool operator!=(const ProblemState&other) const;
  public:
    vector<shared_ptr<ProblemState>> createSuccessorStates();
    bool goalTest();
    void printState();
  public:
    int getCost();
    void setCost(int cost);
  protected:
    int cost;
};

#endif // PROBLEMSTATE_H
