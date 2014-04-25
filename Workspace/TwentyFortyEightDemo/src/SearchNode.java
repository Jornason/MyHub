// This search node class is employed by the various algorithms to store problem
// states. It keeps track of the state and the parent node from which the state
// was derived
public class SearchNode
{
  // The parent node from which this node was derived
  private SearchNode   parent;
  // The state stored in this node
  private ProblemState state;
  // The default constructor
  public SearchNode()
  {
    setState(null);
    setParent(null);
  }
  // A constructor taking a problem state as a parameter
  public SearchNode(ProblemState prob)
  {
    setState(prob);
    setParent(null);
  }
  // A constructor that takes a parent as the argument
  public SearchNode(SearchNode orig)
  {
    setState(orig.getState());
    setParent(orig);
  }
  // A private helper method used by the checkBackTrack() method that determines
  // if the parent of the node has been visited
  private boolean checkAncestor(SearchNode that)
  {
    // If we have been here before, return true (there is backtracking)
    if(getState().equals(that.getState()))
    {
      return true;
    }else
    {
      // Recursively call, passing in the parent, if it exists
      if(that.getParent()!=null){ return checkAncestor(that.getParent()); }
      return false;
    }
  }
  // Determines if the state is already in the path
  public boolean checkBacktrack()
  {
    // If there is a parent check if it is equal
    if(getParent()!=null)
    {
      // Return checkAncestor(getParent());
      return checkAncestor(parent);
    }
    return false;
  }
  // Equal method for Search node
  @Override public boolean equals(Object thatNode)
  {
    SearchNode that=(SearchNode)thatNode;
    if(getState().equals(that.getState())){ return true; }
    return false;
  }
  // Parent getter
  public SearchNode getParent()
  {
    return parent;
  }
  // State getter
  public ProblemState getState()
  {
    return state;
  }
  // Express the cost of the solution. Prints each node in the solution
  public void printSolution()
  {
    // If there is a parent, print it first (call recursively)
    if(getParent()!=null)
    {
      getParent().printSolution();
    }
    // Print the state
    getState().printState();
  }
  // Parent setter
  public void setParent(SearchNode parent)
  {
    this.parent=parent;
  }
  // State setter
  public void setState(ProblemState state)
  {
    this.state=state;
  }
}
