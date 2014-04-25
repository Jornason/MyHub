public class BreadthFirstTwentyFortyEight{
  
  public static void main(String[] args){
    ProblemState initialState;
    SearchNode initialNode;
    initialState=new TwentyFortyEight();
    initialNode=new SearchNode(initialState);
    BreadthFirst.breadthFirst(initialNode,true);
    return;
  }
  
}
