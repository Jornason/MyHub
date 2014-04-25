import java.util.ArrayList;
/*
 * This is an abstract representation of a problem state that will be used by a
 * generic problem solver. It keeps track of the cost of a solution and demands
 * that deriving classes implement various methods to solve the problem.
 */
abstract public class ProblemState
{
  /*
   * The cost of a solution
   */
  private int cost;
  /*
   * Default constructor
   */
  public ProblemState()
  {
    setCost(0);
  }
  /*
   * Copy constructor (used to get a starting point for a derived state)
   */
  public ProblemState(ProblemState orig)
  {
    setCost(orig.getCost());
  }
  /*
   * Is the state legal?
   */
  public abstract boolean isLegal();
  /*
   * derive the child states
   */
  public abstract ArrayList<ProblemState> createSuccessorStates();
  /*
   * print a representation of the state
   */
  public abstract void printState();
  /*
   * is the state the goal state
   */
  public abstract boolean goalTest();
  /*
   * an equals method for the problem state(non-Javadoc)
   * 
   * @see java.lang.Object#equals(java.lang.Object)
   */
  public abstract boolean equals(Object thatState);
  /*
   * get Manhattan distance heuristic (return -1 if not applicable)
   */
  public abstract int manhattanDistance();
  /*
   * get Number out of place heuristic (return -1 if not applicable)
   */
  public abstract int numOutOfPlace();
  /*
   * cost getter
   */
  public int getCost()
  {
    return cost;
  }
  /*
   * cost setter
   */
  public void setCost(int cost)
  {
    this.cost = cost;
  }
}
