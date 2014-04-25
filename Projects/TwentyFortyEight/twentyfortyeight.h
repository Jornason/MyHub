#ifndef TWENTYFORTYEIGHT_H
#define TWENTYFORTYEIGHT_H
#include"problemstate.h"
using namespace std;
class TwentyFortyEight : public ProblemState
{
  public:
    static const unsigned short int goal=2048;
  public:
    TwentyFortyEight();
    TwentyFortyEight(ProblemState*orig);
    ~TwentyFortyEight();
  public:
    TwentyFortyEight operator=(const TwentyFortyEight&that);
    bool operator==(const TwentyFortyEight&that) const;
    bool operator!=(const TwentyFortyEight&that) const;
  public:
    vector<shared_ptr<ProblemState> >createSuccessorStates();
    bool goalTest();
    void printState();
  private:
    void addRandom();
    unsigned short int* packAndCombine(unsigned short int* array);
  private:
    unsigned short int grid[16];
    char direction;
};

#endif // TWENTYFORTYEIGHT_H
