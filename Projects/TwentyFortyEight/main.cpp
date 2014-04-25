#include"searchnode.h"
#include"twentyfortyeight.h"
#include<stack>
using namespace std;

void depthFirst(shared_ptr<SearchNode>&initialNode);

int main()
{
  shared_ptr<ProblemState> initialState
      = shared_ptr<ProblemState>(new TwentyFortyEight());
  shared_ptr<SearchNode> initialNode
      = shared_ptr<SearchNode>(new SearchNode(*initialState));
  depthFirst(initialNode);

}

void depthFirst(shared_ptr<SearchNode> &initialNode)
{
  shared_ptr<SearchNode> pathNode;
  stack<shared_ptr<SearchNode> > path;
  path.push(initialNode);
  int numNodes=0;
  vector<shared_ptr<ProblemState> > derivedStates;
  while(!path.empty())
  {
    SearchNode node = *path.top();
    path.pop();
    numNodes++;
    if(node.getState().goalTest())
    {
      node.printSolution();
      printf("Number of nodes visited = %d\n", numNodes);
      return;
    }
    derivedStates=node.getState().createSuccessorStates();
    for(unsigned char i =0;i<derivedStates.size();i++)
    {
      shared_ptr<SearchNode> newNode = shared_ptr<SearchNode>(new SearchNode(*derivedStates[i]));
      (*newNode).setParent(node);
      if(!(*newNode).checkBacktrack())
      {
        path.push(newNode);
      }
    }
  }
  printf("No solution found\n");
  return;
}

