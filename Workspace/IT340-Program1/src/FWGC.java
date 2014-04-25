import java.util.ArrayList;
/*
 * Farmer, Wolf, Goat, Cabbage Problem state representation
 * The Farmer, Wolf, Goat, Cabbage puzzle is about a farmer trying to move a
 * wolf, a goat, and a cabbage across a river using a ferry that can only hold
 * the farmer and one of the items. While the farmer is on the other side of
 * the river, the wolf cannot be left with the goat because the wolf will eat
 * the goat and the goat cannot be left with the cabbage because the goat will
 * eat the cabbage.
 */
public class FWGC extends ProblemState
{
  /*
   * Booleans to represent the entities. False = on the original side. True = on
   * the other shore
   */
  private boolean f, w, g, c;
  /*
   * Default constructor (all entities on the wrong shore)
   */
  public FWGC()
  {
    super();
    f = w = g = c = false;
  }
  /*
   * Copy constructor
   */
  public FWGC(FWGC orig)
  {
    super(orig);
    f = orig.f;
    w = orig.w;
    g = orig.g;
    c = orig.c;
  }
  /*
   * Is the state legal(non-Javadoc)
   * @see ProblemState#isLegal()
   */
  public boolean isLegal()
  {
    // The wolf has been left with the goat and the farmer is not around
    if (w == g && f != w)
    {
      return false;
    }
    // The goat has been left with the cabbage and the farmer is not around
    if (g == c && f != g)
    {
      return false;
    }
    return true;
  }
  /*
   * create the successor states(non-Javadoc)
   * @see ProblemState#createSuccessorStates()
   */
  public ArrayList<ProblemState> createSuccessorStates()
  {
    // container to hold the states
    ArrayList<ProblemState> states = new ArrayList<ProblemState>(4);
    // Try moving just the farmer
    FWGC movef = new FWGC(this);
    movef.setCost(movef.getCost() + 1);
    movef.f = !movef.f;
    if (movef.isLegal())
    {
      states.add(movef);
    }
    // Try moving the farmer and the wolf
    FWGC movew = new FWGC(this);
    movew.setCost(movew.getCost() + 1);
    movew.f = !movew.f;
    movew.w = !movew.w;
    if (movew.isLegal())
    {
      states.add(movew);
    }
    // Try moving the farmer and the goat
    FWGC moveg = new FWGC(this);
    moveg.setCost(moveg.getCost() + 1);
    moveg.f = !moveg.f;
    moveg.g = !moveg.g;
    if (moveg.isLegal())
    {
      states.add(moveg);
    }
    // Try moving the farmer and the cabbage
    FWGC movec = new FWGC(this);
    movec.setCost(movec.getCost() + 1);
    movec.f = !movec.f;
    movec.c = !movec.c;
    if (movec.isLegal())
    {
      states.add(movec);
    }
    return states;
  }
  /*
   * Print the state(non-Javadoc)
   * @see ProblemState#printState()
   */
  public void printState()
  {
    if (f)
      System.out.println("        |  Farmer");
    else
      System.out.println("Farmer  |        ");
    if (w)
      System.out.println("        |    Wolf");
    else
      System.out.println("Wolf    |        ");
    if (g)
      System.out.println("        |    Goat");
    else
      System.out.println("Goat    |        ");
    if (c)
      System.out.println("        | Cabbage");
    else
      System.out.println("Cabbage |        ");
    System.out.println("Depth: " + getCost());
    System.out.println("-----------------");
  }
  /*
   * Is the state the goal?(non-Javadoc)
   * @see ProblemState#goalTest()
   */
  public boolean goalTest()
  {
    // If all entities are on the right shore, yes
    if (f && w && g && c)
    {
      return true;
    }
    return false;
  }
  /*
   * Equals method(non-Javadoc)
   * @see ProblemState#equals(java.lang.Object)
   */
  public boolean equals(Object thatState)
  {
    FWGC that = (FWGC) thatState;
    if (this.f == that.f && this.w == that.w && this.g == that.g
        && this.c == that.c)
    {
      return true;
    }
    return false;
  }
  /*
   * Get the manhattan distance heuristic for the FWGC puzzle state(non-Javadoc)
   * @see ProblemState#manhattanDistance()
   */
  public int manhattanDistance()
  {
    // not implemented
    return -1;
  }
  /*
   * Get the number out of place for the the FWGC puzzle state(non-Javadoc)
   * @see ProblemState#numOutOfPlace()
   */
  public int numOutOfPlace()
  {
    // not implemented
    return -1;
  }
}
