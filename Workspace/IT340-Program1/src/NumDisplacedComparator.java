import java.util.Comparator;
/*
 * Comparator class used for AStar search. Compares using number of tiles
 * displaced heuristic
 */
public class NumDisplacedComparator implements Comparator<SearchNode>
{
  /*
   * Compare the f scores (cost from start to the node + the estimated cost from
   * the node to the goal
   */
  public int compare(SearchNode first, SearchNode second)
  {
    if (first.getState().numOutOfPlace() + first.getState().getCost() < second
        .getState().numOutOfPlace()
        + second.getState().getCost())
    {
      return -1;
    } else if (first.getState().numOutOfPlace() + first.getState().getCost() > second
        .getState().numOutOfPlace()
        + second.getState().getCost())
    {
      return 1;
    } else
    {
      return 0;
    }
  }
}
