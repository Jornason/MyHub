// Run breadth-first search on the 2048 puzzle
// THIS IS PRETTY MUCH DESIGNED TO FAIL ON ANYTHING BUT THE MOST RIDICULOUSLY
// OVERPOWERED MACHINE WITH MORE RAM THAN PROBABLY EXISTS IN THE WORLD TODAY.
// This is because the theoretical perfect game (randomness is on your side and
// all moves are optimal) still requires 512 moves. I wasn't able to get beyond
// 13-14 moves before all memory was depleted. Additionally, the branching
// factor will probably be a little bit worse than the 8-puzzle.
public class BreadthFirstTwentyFortyEight
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
    // placed at a random spot on the board
    initialState=new TwentyFortyEight();
    initialNode=new SearchNode(initialState);
    // Run breadth first search on it
    BreadthFirst.breadthFirst(initialNode,debug);
    return;
  }
}
