import java.util.Comparator;
/*
 * Comparator class used for AStar search. Compares using Manhattan distance
 * heuristic.
 */
public class ManhattanComparator implements Comparator<SearchNode>
{
  /*
   * Compare the f scores (cost from start to the node + the estimated cost from
   * the node to the goal
   */
  public int compare(SearchNode first, SearchNode second)
  {
    if (first.getState().manhattanDistance() + first.getState().getCost() < second
        .getState().manhattanDistance()
        + second.getState().getCost())
    {
      return -1;
    } else if (first.getState().manhattanDistance()
        + first.getState().getCost() > second.getState().manhattanDistance()
        + second.getState().getCost())
    {
      return 1;
    } else
    {
      return 0;
    }
  }
}
