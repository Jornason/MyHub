import java.util.ArrayList;
// This is an abstract representation of a problem state that will be used by a
// generic problem solver. It keeps track of the cost of a solution and demands
// that deriving classes implement various methods to solve the problem.
abstract public class ProblemState
{
  // The cost of a solution
  private int cost;
  // Default constructor
  public ProblemState()
  {
    setCost(0);
  }
  // Copy constructor (used to get a starting point for a derived state)
  public ProblemState(ProblemState orig)
  {
    setCost(orig.getCost());
  }
  // Derive the child states
  public abstract ArrayList<ProblemState> createSuccessorStates();
  // An equals method for the problem state
  @Override public abstract boolean equals(Object thatState);
  // Cost getter
  public int getCost()
  {
    return cost;
  }
  // is the state the goal state?
  public abstract boolean goalTest();
  // Is the state legal?
  public abstract boolean isLegal();
  // Print a representation of the state
  public abstract void printState();
  // Cost setter
  public void setCost(int cost)
  {
    this.cost=cost;
  }
}
