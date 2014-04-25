/*
 * Farmer Wolf Goat Cabbage Breadth First Test
 * Thomas Lyons
 * IT340
 * Program 1
 */
public class BreadthFirstFWGC
{
  public static void main(String[] args)
  {
    // debug switch
    boolean debug = false;
    // Declare the initial state and node
    ProblemState initialState;
    SearchNode initialNode;
    // parse arguments
    for (int i = 0; i < args.length; i++)
    {
      // debug mode
      if (args[i].compareToIgnoreCase("debug") == 0)
      {
        debug = true;
      }
    }
    initialState = new FWGC();
    // initialize the search node
    initialNode = new SearchNode(initialState);
    // We are doing breadth first
    BreadthFirst.breadthFirst(initialNode, debug);
    return;
  }
}