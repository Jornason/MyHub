/*
 * This search node class is employed by the various algorithms to store
 * problem states. It keeps track of the state and the parent node from which
 * the state was derived
 */
public class SearchNode
{
  /*
   * The parent node from which this node was derived
   */
  private SearchNode   parent;
  /*
   * The state stored in this node
   */
  private ProblemState state;
  /*
   * The default constructor
   */
  public SearchNode()
  {
    setState(null);
    setParent(null);
  }
  /*
   * A constructor taking a problem state as a parameter
   */
  public SearchNode(ProblemState prob)
  {
    setState(prob);
    setParent(null);
  }
  /*
   * A constructor that takes a parent as the argument
   */
  public SearchNode(SearchNode orig)
  {
    setState(orig.getState());
    setParent(orig);
  }
  /*
   * Express the cost of the solution. Prints each node in the solution
   */
  public void printSolution()
  {
    // If there is a parent, print it first (call recursively)
    if (this.getParent() != null)
    {
      getParent().printSolution();
    }
    // print the state
    this.getState().printState();
  }
  /*
   * Determines if the state is already in the path
   */
  public boolean checkBacktrack()
  {
    // If there is a parent check if it is equal
    if (getParent() != null)
    {
      // return checkAncestor(getParent());
      return checkAncestor(parent);
    }
    return false;
  }
  /*
   * override the object equals method(non-Javadoc)
   * 
   * @see java.lang.Object#equals(java.lang.Object)
   */
  public boolean equals(Object thatNode)
  {
    SearchNode that = (SearchNode) thatNode;
    if (this.getState().equals(that.getState()))
    {
      return true;
    }
    return false;
  }
  /*
   * parent getter
   */
  public SearchNode getParent()
  {
    return parent;
  }
  /*
   * parent setter
   */
  public void setParent(SearchNode parent)
  {
    this.parent = parent;
  }
  /*
   * state getter
   */
  public ProblemState getState()
  {
    return state;
  }
  /*
   * state setter
   */
  public void setState(ProblemState state)
  {
    this.state = state;
  }
  /*
   * A private helper method used by the checkBackTrack() method that determines
   * if the parent of the node has been visited
   */
  private boolean checkAncestor(SearchNode that)
  {
    // If we have been here before, return true (there is backtracking)
    if (this.getState().equals(that.getState()))
    {
      return true;
    } else
    {
      // recursively call, passing in the parent, if it exists
      if (that.getParent() != null)
      {
        return checkAncestor(that.getParent());
      }
      return false;
    }
  }
}
