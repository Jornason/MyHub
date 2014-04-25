public class GreedySearchTwentyFortyEight{
  
  public static void main(String[] args){
    ProblemState initialState;
    SearchNode initialNode;
    initialState=new TwentyFortyEight();
    initialNode=new SearchNode(initialState);
    GreedySearch.greedy(initialNode,true);
  }
  
}
