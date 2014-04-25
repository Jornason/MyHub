import java.util.ArrayList;
import java.util.LinkedList;

public class GreedySearch{
  public static void greedy(SearchNode initialNode,boolean debug){
    int i;
    LinkedList<SearchNode> path=new LinkedList<SearchNode>();
    path.add(initialNode);
    int numNodes=0;
    ArrayList<ProblemState> derivedStates;
    do{
      numNodes++;
      SearchNode node=path.poll();
      node.getState().printState();
      if(node.getState().goalTest()){
        System.out.println("SUCCESS!!!");
        System.out.println("Number Of moves = "+numNodes);
        return;
      }
      derivedStates=node.getState().createSuccessorStates();
      boolean lookingForPath=true;
      while(lookingForPath){
        if(derivedStates.size()==0){
          break;
        }
        int bestCost=derivedStates.get(0).getCost();
        int bestIndex=0;
        
        for(i=1;i<derivedStates.size();i++){
          if(derivedStates.get(i).getCost()<bestCost){
            bestCost=derivedStates.get(i).getCost();
            bestIndex=i;
          }
        }
        SearchNode newNode=new SearchNode(derivedStates.get(bestIndex));
        newNode.setParent(node);
        // if(!newNode.checkBacktrack()){
        path.add(newNode);
        lookingForPath=false;
        // }else{
        // derivedStates.remove(bestIndex);
        // }
      }
    }while(derivedStates.size()!=0);
    System.out.println("FAILED");
    System.out.println("Number Of moves = "+numNodes);
    return;
  }
  
}
