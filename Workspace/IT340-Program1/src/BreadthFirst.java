import java.util.ArrayList;
import java.util.LinkedList;
/*
 * Breadth first search solves the problem by visiting each node of equal
 * depth before pressing further into the path. It differs from depth first
 * search only in its use of a queue instead of a stack
 */
public class BreadthFirst
{
  public static void breadthFirst(SearchNode initialNode, boolean debug)
  {
    // Declare a queue to hold the current path
    LinkedList<SearchNode> path = new LinkedList<SearchNode>();
    // Add the initial node to the path
    path.add(initialNode);
    // initialize the number of nodes
    int numNodes = 0;
    // Declare the derived states
    ArrayList<ProblemState> derivedStates;
    // Continue until there are no more paths or a solution is found
    while (path.size() != 0)
    {
      // pop a node from the queue
      SearchNode node = path.poll();
      // increment the number of nodes visited
      numNodes++;
      // If this is the solution node, print the solution and return
      if (node.getState().goalTest())
      {
        node.printSolution();
        if (debug)
        {
          System.out.println("Number of nodes visited: " + numNodes);
        }
        return;
      }
      // otherwise, create the node's derived states
      derivedStates = node.getState().createSuccessorStates();
      int i;
      // for each derived state...
      for (i = 0; i < derivedStates.size(); i++)
      {
        // create nodes for each state
        SearchNode newNode = new SearchNode(derivedStates.get(i));
        // set the parent of the derived node to the current node
        newNode.setParent(node);
        // If the node isn't backtracking, push it
        if (!newNode.checkBacktrack())
        {
          path.add(newNode);
        }
      }
    }
    System.out.println("No solution found");
    return;
  }
}
