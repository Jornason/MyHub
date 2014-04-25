import java.util.ArrayList;
import java.util.Random;

public class TwentyFortyEight extends ProblemState{
  
  private int[][] grid =new int[4][4];
  private String  direction;
  private int     numInPath;
  
  public TwentyFortyEight(){
    super();
    numInPath=1;
    int i,j;
    // set the pieces;
    for(i=0;i<4;i++){
      for(j=0;j<4;j++){
        grid[i][j]=-1;
      }
    }
    addRandom();
  }
  
  public TwentyFortyEight(TwentyFortyEight orig){
    super(orig);
    numInPath=orig.numInPath+1;
    int i,j;
    for(i=0;i<3;i++){
      for(j=0;j<3;j++){
        grid[i][j]=orig.grid[i][j];
      }
    }
  }
  
  private void addRandom(){
    Random rand=new Random();
    int nextNum=0;
    int randomNum=rand.nextInt(2);
    if(randomNum==0){
      nextNum=2;
    }else{
      nextNum=4;
    }
    // find the potential places to place the new tile
    int i,j;
    ArrayList<Integer> potentialPlaces=new ArrayList<Integer>();
    for(i=0;i<4;i++){
      for(j=0;j<4;j++){
        if(grid[i][j]==-1){
          potentialPlaces.add(i*4+j);
        }
      }
    }
    if(potentialPlaces.size()==0){ return; }
    randomNum=rand.nextInt(potentialPlaces.size());
    
    int location=potentialPlaces.get(randomNum);
    grid[location/4][location%4]=nextNum;
  }
  
  private int countNumOfFreeSpaces(){
    int count=0;
    int i,j;
    for(i=0;i<4;i++){
      for(j=0;j<4;j++){
        if(grid[i][j]==-1){
          count++;
        }
      }
    }
    return count;
  }
  
  @Override public ArrayList<ProblemState> createSuccessorStates(){
    ArrayList<ProblemState> states=new ArrayList<ProblemState>(4);
    TwentyFortyEight left=new TwentyFortyEight(this);
    int i;
    // check left to right
    for(i=0;i<4;i++){
      int[] array=new int[4];
      array[0]=grid[i][0];
      array[1]=grid[i][1];
      array[2]=grid[i][2];
      array[3]=grid[i][3];
      packAndCombine(array);
      left.grid[i][0]=array[0];
      left.grid[i][1]=array[1];
      left.grid[i][2]=array[2];
      left.grid[i][3]=array[3];
    }
    left.setCost(16-countNumOfFreeSpaces());
    left.direction="left";
    if(!equals(left)){
      left.addRandom();
      states.add(left);
    }
    
    TwentyFortyEight right=new TwentyFortyEight(this);
    // check right to left
    for(i=0;i<4;i++){
      int[] array=new int[4];
      array[0]=grid[i][3];
      array[1]=grid[i][2];
      array[2]=grid[i][1];
      array[3]=grid[i][0];
      packAndCombine(array);
      right.grid[i][3]=array[0];
      right.grid[i][2]=array[1];
      right.grid[i][1]=array[2];
      right.grid[i][0]=array[3];
    }
    right.setCost(16-countNumOfFreeSpaces());
    right.direction="right";
    if(!equals(right)){
      right.addRandom();
      states.add(right);
    }
    
    TwentyFortyEight up=new TwentyFortyEight(this);
    // check up to down
    for(i=0;i<4;i++){
      int[] array=new int[4];
      array[0]=grid[0][i];
      array[1]=grid[1][i];
      array[2]=grid[2][i];
      array[3]=grid[3][i];
      packAndCombine(array);
      up.grid[0][i]=array[0];
      up.grid[1][i]=array[1];
      up.grid[2][i]=array[2];
      up.grid[3][i]=array[3];
    }
    up.direction="up";
    up.setCost(16-countNumOfFreeSpaces());
    if(!equals(up)){
      up.addRandom();
      states.add(up);
    }
    
    TwentyFortyEight down=new TwentyFortyEight(this);
    // check down to up
    for(i=0;i<4;i++){
      int[] array=new int[4];
      array[0]=grid[3][i];
      array[1]=grid[2][i];
      array[2]=grid[1][i];
      array[3]=grid[0][i];
      packAndCombine(array);
      down.grid[3][i]=array[0];
      down.grid[2][i]=array[1];
      down.grid[1][i]=array[2];
      down.grid[0][i]=array[3];
    }
    down.direction="down";
    down.setCost(16-countNumOfFreeSpaces());
    if(!equals(down)){
      down.addRandom();
      states.add(down);
    }
    return states;
  }
  
  @Override public boolean equals(Object obj){
    if(obj==null){ return false; }
    if(getClass()!=obj.getClass()){ return false; }
    final TwentyFortyEight that=(TwentyFortyEight)obj;
    int i,j;
    for(i=0;i<4;i++){
      for(j=0;j<4;j++){
        if(grid[i][j]!=that.grid[i][j]){ return false; }
      }
    }
    return true;
  }
  
  @Override public boolean goalTest(){
    int i,j;
    for(i=0;i<4;i++){
      for(j=0;j<4;j++){
        if(grid[i][j]==8192){ return true; }
      }
    }
    return false;
  }
  
  @Override public boolean isLegal(){
    return true;
  }
  
  @Override public int manhattanDistance(){
    // not implemented
    return -1;
  }
  
  @Override public int numOutOfPlace(){
    // not implemented
    return -1;
  }
  
  private int[] packAndCombine(int[] array){
    int j=0;
    while(j<4){
      // pack them in tightly
      if(array[j]==-1){
        int savedJ=j;
        while(j<4){
          if(array[j]==-1){
            j++;
          }else{
            array[savedJ]=array[j];
            array[j]=-1;
            break;
          }
        }
        j=savedJ;
      }
      // try to combine
      if(j!=3&&array[j]!=-1&&array[j]==array[j+1]){
        array[j]*=2;
        array[j+1]=-1;
      }
      j++;
    }
    return array;
  }
  
  @Override public void printState(){
    System.out.println("+------+------+------+------+  "+direction
        +" "
        +numInPath);
    int i,j;
    for(i=0;i<4;i++){
      System.out.println("|      |      |      |      |");
      System.out.print("|");
      for(j=0;j<4;j++){
        if(grid[i][j]==-1){
          System.out.print("      |");
        }else{
          System.out.printf(" %4d |",grid[i][j]);
        }
      }
      System.out.println();
      System.out.println("|      |      |      |      |");
      System.out.println("+------+------+------+------+");
    }
  }
  
}
