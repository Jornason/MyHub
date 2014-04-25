import java.util.ArrayList;
/*
 * Eight Puzzle Problem State representation.
 * The class extends the problem state class, implements all abstract methods,
 * and is used to hold a state for an 8-puzzle problem.
 * The eight puzzle problem demands that the solver arrange a 3 x 3 grid with
 * 8 numbered tiles and one blank so that all the numbers are in order (left to
 * right, up to down) and that the blank is at the lower right corner. This must
 * be achieved by switching the blank space with an adjacent tile in one of the
 * 4 cardinal direction. If there is a wall on one side, then that direction
 * cannot be used.
 */
public class EightPuzzle extends ProblemState
{
  /*
   * 3x3 grid representing puzzle
   */
  private int[][] grid = new int[3][3];
  /*
   * Indexes used to keep track of the location of the blank
   */
  private int     spaceX;
  private int     spaceY;
  /*
   * Default constructor (already solved)
   */
  public EightPuzzle()
  {
    // call the super constructor
    super();
    int i, j;
    // set the pieces
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        grid[i][j] = (i * 3) + j + 1;
      }
    }
    // initialize the indices
    spaceX = 2;
    spaceY = 2;
  }
  /*
   * Copy constructor
   */
  public EightPuzzle(EightPuzzle orig)
  {
    // call the super constructor
    super(orig);
    int i, j;
    // set up the grid
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        grid[i][j] = orig.grid[i][j];
      }
    }
    // copy the indices
    spaceX = orig.spaceX;
    spaceY = orig.spaceY;
  }
  /*
   * Constructor from a arguments
   */
  public EightPuzzle(int[] numArray)
  {
    // call the super constructor
    super();
    int i, j;
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        if (numArray[i * 3 + j] == 0 || numArray[i * 3 + j] == 9)
        {
          grid[i][j] = 9;
          spaceY = i;
          spaceX = j;
        } else
        {
          grid[i][j] = numArray[i * 3 + j];
        }
      }
    }
  }
  /*
   * Is the state legal(non-Javadoc)
   * @see ProblemState#isLegal()
   */
  public boolean isLegal()
  {
    return true;
  }
  /*
   * Create successor states(non-Javadoc)
   * @see ProblemState#createSuccessorStates()
   */
  public ArrayList<ProblemState> createSuccessorStates()
  {
    // Declare an arraylist to hold the states
    ArrayList<ProblemState> states = new ArrayList<ProblemState>(4);
    /*
     * Uncomment these lines if you want the program to actually, you know,
     * work....
     */
    // if(cost >= 31){ return children;}
    // moveLeft
    if (spaceX != 0)
    {
      EightPuzzle left = new EightPuzzle(this);
      left.setCost(this.getCost() + 1);
      left.grid[spaceY][spaceX] = left.grid[spaceY][spaceX - 1];
      left.grid[spaceY][spaceX - 1] = 9;
      left.spaceX--;
      if (left.isLegal())
      {
        states.add(left);
      }
    }
    // moveDown
    if (spaceY != 2)
    {
      EightPuzzle down = new EightPuzzle(this);
      down.setCost(this.getCost() + 1);
      down.grid[spaceY][spaceX] = down.grid[spaceY + 1][spaceX];
      down.grid[spaceY + 1][spaceX] = 9;
      down.spaceY++;
      if (down.isLegal())
      {
        states.add(down);
      }
    }
    // moveRight
    if (spaceX != 2)
    {
      EightPuzzle right = new EightPuzzle(this);
      right.setCost(this.getCost() + 1);
      right.grid[spaceY][spaceX] = right.grid[spaceY][spaceX + 1];
      right.grid[spaceY][spaceX + 1] = 9;
      right.spaceX++;
      if (right.isLegal())
      {
        states.add(right);
      }
    }
    // moveUp;
    if (spaceY != 0)
    {
      EightPuzzle up = new EightPuzzle(this);
      up.setCost(this.getCost() + 1);
      up.grid[spaceY][spaceX] = up.grid[spaceY - 1][spaceX];
      up.grid[spaceY - 1][spaceX] = 9;
      up.spaceY--;
      if (up.isLegal())
      {
        states.add(up);
      }
    }
    return states;
  }
  /*
   * print the state(non-Javadoc)
   * @see ProblemState#printState()
   */
  public void printState()
  {
    int i, j;
    System.out.println("+---+---+---+");
    for (i = 0; i < 3; i++)
    {
      System.out.print("| ");
      for (j = 0; j < 3; j++)
      {
        if (grid[i][j] == 9)
        {
          System.out.print("  | ");
        } else
        {
          System.out.print(grid[i][j] + " | ");
        }
      }
      System.out.println();
      System.out.println("+---+---+---+");
    }
    System.out.println("Depth: " + this.getCost());
    System.out.println();
    System.out.println();
  }
  /*
   * is the state the goal state?(non-Javadoc)
   * @see ProblemState#goalTest()
   */
  public boolean goalTest()
  {
    int i, j;
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        if (grid[i][j] != (i * 3) + j + 1)
        {
          return false;
        }
      }
    }
    return true;
  }
  /*
   * Check if this state is the equal to another state(non-Javadoc)
   * @see ProblemState#equals(java.lang.Object)
   */
  public boolean equals(Object thatState)
  {
    EightPuzzle that = (EightPuzzle) thatState;
    int i, j;
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        if (this.grid[i][j] != that.grid[i][j])
        {
          return false;
        }
      }
    }
    return true;
  }
  /*
   * Get the manhattan distance heuristic for the EP puzzle state(non-Javadoc)
   * @see ProblemState#manhattanDistance()
   */
  public int manhattanDistance()
  {
    int totalDistance = 0;
    int i, j;
    // perform for each tile
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        int numInTile = grid[i][j];
        if (numInTile == 9)
        {
          continue;
        }
        int targetX = (numInTile - 1) % 3;
        int targetY = (numInTile - 1) / 3;
        int dx = j - targetX;
        int dy = i - targetY;
        totalDistance += Math.abs(dx) + Math.abs(dy);
      }
    }
    return totalDistance;
  }
  /*
   * Get the number out of place for the the EP puzzle state(non-Javadoc)
   * @see ProblemState#numOutOfPlace()
   */
  public int numOutOfPlace()
  {
    int numDisplaced = 0;
    int i, j;
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; j++)
      {
        if (grid[i][j] == 9)
        {
          continue;
        }
        if (grid[i][j] != i * 3 + j + 1)
        {
          numDisplaced++;
        }
      }
    }
    return numDisplaced;
  }
}
