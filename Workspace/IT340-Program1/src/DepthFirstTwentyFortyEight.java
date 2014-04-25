public class DepthFirstTwentyFortyEight
{
  public static void main(String[] args)
  {
    boolean debug=false;
    ProblemState initialState;
    SearchNode initialNode;
    // Read the arguments (looking for debug)
    for(String arg:args)
    {
      // debug mode
      if(arg.compareToIgnoreCase("debug")==0)
      {
        debug=true;
      }
    }
    // Create the initial state (blank board with one random tail (2 or 4)
    // placed
    // at a random spot on the board
    initialState=new TwentyFortyEight();
    initialNode=new SearchNode(initialState);
    // Run depth first search on it
    DepthFirst.depthFirst(initialNode,debug);
  }
}
