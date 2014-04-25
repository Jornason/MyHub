/*
 * Depth First Eight Puzzle Test
 * Thomas Lyons
 * IT340
 * Program 1
 */
public class DepthFirstEightPuzzle
{
  public static void main(String[] args)
  {
    // the debug switch
    boolean debug = false;
    // Declare the initial state and node
    ProblemState initialState;
    SearchNode initialNode;
    // array of numbers in eight puzzle
    int[] numArray =
    { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int numIndex = 0;
    // parse arguments
    for (int i = 0; i < args.length; i++)
    {
      // debug mode
      if (args[i].compareToIgnoreCase("debug") == 0)
      {
        debug = true;
      }
      if (Character.isDigit(args[i].charAt(0)))
      {
        numArray[numIndex] = Integer.parseInt(args[i]);
        numIndex++;
      }
    }
    initialState = new EightPuzzle(numArray);
    // initialize the search node
    initialNode = new SearchNode(initialState);
    // We are doing depth first
    DepthFirst.depthFirst(initialNode, debug);
    return;
  }
}