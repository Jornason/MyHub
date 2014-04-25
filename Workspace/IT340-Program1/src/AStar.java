import java.util.ArrayList;
import java.util.PriorityQueue;
/*
 * A-star search. Uses heuristics to find the best path.
 * 
 * 
 */
public class AStar
{
  public static void aStar(SearchNode initialNode, int flag, boolean debug)
  {
    // Declare a priority queue to hold nodes that we can travel to
    PriorityQueue<SearchNode> openSet;
    // Declare a list to hold the nodes that we have already traveled to
    ArrayList<SearchNode> closedSet = new ArrayList<SearchNode>();
    if (flag == 0)
    {
      // use number of tiles displaced as a heuristic
      openSet = new PriorityQueue<SearchNode>(32,
          new NumDisplacedComparator());
    } else
    {
      // use manhattan distance as a heuristic
      openSet = new PriorityQueue<SearchNode>(32, new ManhattanComparator());
    }
    // Add the initial node to the list of open nodes
    openSet.add(initialNode);
    // initialize the number of nodes
    int numNodes = 0;
    // Declare the derived states
    ArrayList<ProblemState> derivedStates;
    // Continue until there are no more paths or a solution is found
    while (openSet.size() > 0)
    {
      // pop a node from the list of navigable nodes
      SearchNode node = openSet.poll();
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
      // otherwise, add the node to the list of nodes we have visited
      closedSet.add(node);
      // create the node's derived states
      derivedStates = node.getState().createSuccessorStates();
      // for each derived state...
      for (int i = 0; i < derivedStates.size(); i++)
      {
        // create a node for the state
        SearchNode derivedNode = new SearchNode(derivedStates.get(i));
        // set the parent of the derived node to the current node
        derivedNode.setParent(node);
        // if the derived node is not in the list of closed nodes
        if (!closedSet.contains(derivedNode))
        {
          /*
           * if the derived node is not in the list of open nodes or the cost to
           * get to the parent node is less than the cost to get to the derived
           * node
           */
          if (!openSet.contains(derivedNode)
              || node.getState().getCost() < derivedNode.getState().getCost())
          {
            // add the node to the open set
            openSet.add(derivedNode);
          }
        }
      }
    }
    System.out.println("No solution found");
  }
}
