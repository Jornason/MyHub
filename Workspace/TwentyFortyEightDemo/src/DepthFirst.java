import java.util.ArrayList;
import java.util.Stack;
// Depth-first search implementation
public class DepthFirst
{
  public static void depthFirst(SearchNode initialNode,boolean debug)
  {
    // Declare a stack to hold the current path
    Stack<SearchNode> path=new Stack<SearchNode>();
    // Add the initial node to the path
    path.push(initialNode);
    // initialize the number of nodes
    int numNodes=0;
    // Declare the derived states
    ArrayList<ProblemState> derivedStates;
    // Continue until there are no more paths or a solution is found
    while(!path.empty())
    {
      // pop a node from the stack
      SearchNode node=path.pop();
      // increment the number of nodes visited
      numNodes++;
      // If this is the solution node, print the solution and return
      if(node.getState().goalTest())
      {
        node.printSolution();
        if(debug)
        {
          System.out.println("Number of nodes visited: "+numNodes);
        }
        return;
      }
      // otherwise, create the node's derived states
      derivedStates=node.getState().createSuccessorStates();
      int i;
      // for each derived state...
      for(i=0;i<derivedStates.size();i++)
      {
        // create nodes for each state
        SearchNode newNode=new SearchNode(derivedStates.get(i));
        // set the parent of the derived node to the current node
        newNode.setParent(node);
        // If the node isn't backtracking, push it
        if(!newNode.checkBacktrack())
        {
          path.push(newNode);
        }
      }
    }
    System.out.println("No solution found");
    return;
  }
}