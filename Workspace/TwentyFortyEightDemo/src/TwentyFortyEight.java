import java.util.ArrayList;
import java.util.Random;
public class TwentyFortyEight extends ProblemState
{
  // The grid that contains the game board
  private int[][] grid =new int[4][4];
  // The direction of the last move
  private String  direction;
  // Initial state constructor
  public TwentyFortyEight()
  {
    super();
    int i,j;
    // set the pieces;
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        // -1 is blank
        grid[i][j]=-1;
      }
    }
    // Add a random tile to the board in a blank spot
    addRandom();
  }
  // Derived state constructor
  public TwentyFortyEight(TwentyFortyEight orig)
  {
    super(orig);
    setCost(orig.getCost()+1);
    int i,j;
    // Copy all of the pieces (don't add a random yet)
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        grid[i][j]=orig.grid[i][j];
      }
    }
  }
  private void addRandom()
  {
    // Add either a 2 or a 4 to a random blank spot on the board
    Random rand=new Random();
    int nextNum=0;
    int randomNum=rand.nextInt(2);
    if(randomNum==0)
    {
      nextNum=2;
    }else
    {
      nextNum=4;
    }
    // find the potential places to place the new tile
    int i,j;
    ArrayList<Integer> potentialPlaces=new ArrayList<Integer>();
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        if(grid[i][j]==-1)
        {
          // Add a potential place to put the new tile
          potentialPlaces.add(i*4+j);
        }
      }
    }
    if(potentialPlaces.size()==0){ return; }
    randomNum=rand.nextInt(potentialPlaces.size());
    int location=potentialPlaces.get(randomNum);
    // Place the new tile in a random but acceptable location (don't overwrite a
    // non-blank cell)
    grid[location/4][location%4]=nextNum;
  }
  // Get all of the moves that we can go to from this state
  @Override public ArrayList<ProblemState> createSuccessorStates()
  {
    ArrayList<ProblemState> states=new ArrayList<ProblemState>(4);
    // Swipe left
    TwentyFortyEight left=new TwentyFortyEight(this);
    int i;
    // Check each row left to right looking for combos and packing things
    // together
    for(i=0;i<4;i++)
    {
      int[] array=new int[4];
      array[0]=grid[i][0];
      array[1]=grid[i][1];
      array[2]=grid[i][2];
      array[3]=grid[i][3];
      // Pack the tiles together then look for tiles to combine
      array=packAndCombine(array);
      left.grid[i][0]=array[0];
      left.grid[i][1]=array[1];
      left.grid[i][2]=array[2];
      left.grid[i][3]=array[3];
    }
    left.direction="left";
    // Ensure that the new state gets us somewhere useful before adding it
    if(!equals(left))
    {
      // Add a random tile
      left.addRandom();
      states.add(left);
    }
    // Swipe right
    TwentyFortyEight right=new TwentyFortyEight(this);
    // Check each row right to left looking for combos and packing things
    // together
    for(i=0;i<4;i++)
    {
      int[] array=new int[4];
      array[0]=grid[i][3];
      array[1]=grid[i][2];
      array[2]=grid[i][1];
      array[3]=grid[i][0];
      // Pack the tiles together then look for tiles to combine
      array=packAndCombine(array);
      right.grid[i][3]=array[0];
      right.grid[i][2]=array[1];
      right.grid[i][1]=array[2];
      right.grid[i][0]=array[3];
    }
    right.direction="right";
    // Ensure that the new state gets us somewhere useful before adding it
    if(!equals(right))
    {
      right.addRandom();
      states.add(right);
    }
    // Swipe up
    TwentyFortyEight up=new TwentyFortyEight(this);
    // Check each column up to down looking for combos and packing things
    // together
    for(i=0;i<4;i++)
    {
      int[] array=new int[4];
      array[0]=grid[0][i];
      array[1]=grid[1][i];
      array[2]=grid[2][i];
      array[3]=grid[3][i];
      // Pack the tiles together then look for tiles to combine
      array=packAndCombine(array);
      up.grid[0][i]=array[0];
      up.grid[1][i]=array[1];
      up.grid[2][i]=array[2];
      up.grid[3][i]=array[3];
    }
    up.direction="up";
    // Ensure that the new state gets us somewhere useful before adding it
    if(!equals(up))
    {
      up.addRandom();
      states.add(up);
    }
    // Swipe down
    TwentyFortyEight down=new TwentyFortyEight(this);
    // Check each column down to up looking for combos and packing things
    // together
    for(i=0;i<4;i++)
    {
      int[] array=new int[4];
      array[0]=grid[3][i];
      array[1]=grid[2][i];
      array[2]=grid[1][i];
      array[3]=grid[0][i];
      // Pack the tiles together then look for tiles to combine
      array=packAndCombine(array);
      down.grid[3][i]=array[0];
      down.grid[2][i]=array[1];
      down.grid[1][i]=array[2];
      down.grid[0][i]=array[3];
    }
    down.direction="down";
    // Ensure that the new state gets us somewhere useful before adding it
    if(!equals(down))
    {
      down.addRandom();
      states.add(down);
    }
    return states;
  }
  // Overridden equals method for the board state
  @Override public boolean equals(Object obj)
  {
    if(obj==null){ return false; }
    if(getClass()!=obj.getClass()){ return false; }
    final TwentyFortyEight that=(TwentyFortyEight)obj;
    int i,j;
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        // If one tile is not the same, they are not equal
        if(grid[i][j]!=that.grid[i][j]){ return false; }
      }
    }
    return true;
  }
  // Have we reached the goal?
  @Override public boolean goalTest()
  {
    int i,j;
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        // We are going to 2048 (changed to 4096, 8192, etc for more difficult
        // problem
        if(grid[i][j]==2048){ return true; }
      }
    }
    return false;
  }
  @Override public boolean isLegal()
  {
    // Not useful in this case
    return true;
  }
  // Pack tiles in the direction that we are swiping and combine them
  private int[] packAndCombine(int[] array)
  {
    int j=0;
    while(j<4)
    {
      // Start at one side of the row/column looking for a blank then shifting
      // following tiles towards it
      if(array[j]==-1)
      {
        // Save the iterator
        int savedJ=j;
        // Look for the next non-blank
        while(j<4)
        {
          if(array[j]==-1)
          {
            j++;
          }else
          {
            // Move the non-blank tile down
            array[savedJ]=array[j];
            array[j]=-1;
            break;
          }
        }
        // Restore the iterator
        j=savedJ;
      }
      // Try to combine (tiles must not be blank, must be equal, and must be
      // adjacent
      // if(j!=3&&array[j]!=-1&&array[j]==array[j+1])
      // {
      // Multiply the original tile by two
      // array[j]*=2;
      // Clear the other tile
      // array[j+1]=-1;
      // }
      // Move to the next file (we can now ignore the one we just came from)
      j++;
    }
    for(j=0;j<3;j++)
    {
      if(array[j]!=-1&&array[j]==array[j+1])
      {
        array[j]*=2;
        for(int k=j+1;k<3;k++)
        {
          array[k]=array[k+1];
        }
        array[3]=-1;
      }
    }
    return array;
  }
  // Print the state in a readable format
  @Override public void printState()
  {
    System.out.println("+------+------+------+------+  "+direction);
    int i,j;
    for(i=0;i<4;i++)
    {
      System.out.println("|      |      |      |      |");
      System.out.print("|");
      for(j=0;j<4;j++)
      {
        if(grid[i][j]==-1)
        {
          System.out.print("      |");
        }else
        {
          // Format to four digits in the printed result
          System.out.printf(" %4d |",grid[i][j]);
        }
      }
      System.out.println();
      System.out.println("|      |      |      |      |");
      System.out.println("+------+------+------+------+");
    }
    System.out.println("Moves made = "+getCost());
  }
}
