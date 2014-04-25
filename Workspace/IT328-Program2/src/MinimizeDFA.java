import java.util.ArrayList;
import java.util.TreeSet;
public class MinimizeDFA
{
  public static DFA minimizeDFA(DFA dfa)
  {
    EClosure originalStartState=new EClosure();
    int n=0;
    for(EClosure m:dfa.stateList)
    {
      if(n==dfa.startingState)
      {
        originalStartState=m;
      }
      n++;
    }
    boolean[][] minimization=new boolean[dfa.numberOfStates][dfa.numberOfStates];
    // list of states in order for comparisons
    ArrayList<EClosure> states=new ArrayList<EClosure>();
    for(EClosure e:dfa.stateList)
    {
      states.add(e);
    }
    /* iterations to determine minimization */
    for(int i=0;i<dfa.numberOfStates;i++)
    {
      /* iteration over row of states */
      for(int j=0;j<dfa.numberOfStates;j++)
      {
        /* iteration over columns (symmetric) */
        for(int k=j;k<dfa.numberOfStates;k++)
        {
          EClosure stateJ=states.get(j);
          EClosure stateK=states.get(k);
          /* Do nothing if state is checking against itself */
          if(j==k)
          {
            ;
          }else if(dfa.finalStates.contains(j)&&!dfa.finalStates.contains(k)
              ||dfa.finalStates.contains(k)
              &&!dfa.finalStates.contains(j))
          {
            minimization[j][k]=true;
            minimization[k][j]=true;
          }
          /*
           * or if the pair+movement is already flagged for any transition.
           * Go through each alphabet option for each and check if this new
           * combination is a flagged already.
           */
          else
          {
            for(int l=0;l<dfa.alphabet.size();l++)
            {
              MoveFunction jMove=null;
              MoveFunction kMove=null;
              for(MoveFunction search:dfa.DFAMoveFunctions)
              {
                if(search.from.equals(stateJ)&&search.index==l)
                {
                  jMove=search;
                }else if(search.from.equals(stateK)&&search.index==l)
                {
                  kMove=search;
                }
              }
              if(jMove!=null&&kMove!=null)
              {
                EClosure jDestination=jMove.to;
                EClosure kDestination=kMove.to;
                if(minimization[states.indexOf(jDestination)][states.indexOf(kDestination)])
                {
                  minimization[j][k]=true;
                  minimization[k][j]=true;
                }
              }
              /* If one has a move function the other is missing, distinct */
              else if(jMove==null&&kMove!=null||jMove!=null&&kMove==null)
              {
                minimization[j][k]=true;
                minimization[k][j]=true;
              }
            }
          }
        }
      }
    }
    /*
     * Examine table for false flags which indicate a collapse is
     * possible and reconstruct DFA.
     */
    ArrayList<Integer> removedFinalStates=new ArrayList<Integer>();
    ArrayList<MoveFunction> movementRemovals=new ArrayList<MoveFunction>();
    ArrayList<Integer> stateRemovals=new ArrayList<Integer>();
    for(int i=0;i<minimization.length;i++)
    {
      for(int j=i;j<minimization[0].length;j++)
      {
        if(i!=j&&minimization[i][j]==false
            &&dfa.stateList.contains(states.get(j)))
        {
          // Remove transitions beginning and move ones ending at j
          for(MoveFunction movement:dfa.DFAMoveFunctions)
          {
            if(movement.to.equals(states.get(j)))
            {
              movement.to=states.get(i);
            }
            if(movement.from.equals(states.get(j)))
            {
              movementRemovals.add(movement);
            }
          }
          // Set j to be removed from finalStates if it is final
          removedFinalStates.add(j);
          // Decrement startingState if greater than j
          if(dfa.startingState>j&&dfa.stateList.contains(states.get(j)))
          {
            dfa.startingState--;
          }
          // Remove j from stateList and decrease numberOfStates
          // Also, mark it as one of the states removed for lowering final
          // states later
          if(dfa.stateList.contains(states.get(j)))
          {
            stateRemovals.add(j);
            dfa.stateList.remove(states.get(j));
            dfa.numberOfStates--;
          }
        }
      }
    }
    // Remove marked final states
    for(Integer removedState:removedFinalStates)
    {
      dfa.finalStates.remove(removedState);
    }
    // Decrement remaining final states by appropriate amount, then replace the
    // old final state set
    TreeSet<Integer> finalStates=new TreeSet<Integer>();
    for(Integer finalState:dfa.finalStates)
    {
      int lowerAmount=0;
      for(Integer state:stateRemovals)
      {
        if(state<finalState)
        {
          lowerAmount++;
        }
      }
      finalStates.add(finalState-lowerAmount);
    }
    dfa.finalStates=finalStates;
    for(MoveFunction mf:movementRemovals)
    {
      dfa.DFAMoveFunctions.remove(mf);
    }
    /* Set the transition table */
    dfa.transitionTable=new ArrayList<ArrayList<TreeSet<Integer>>>();
    for(int i=0;i<dfa.numberOfStates;i++)
    {
      ArrayList<TreeSet<Integer>> stateDestinations=new ArrayList<TreeSet<Integer>>();
      int m=0;
      EClosure thisState=new EClosure();
      for(EClosure state:dfa.stateList)
      {
        if(i==m)
        {
          thisState=state;
          break;
        }
        m++;
      }
      for(int j=0;j<dfa.alphabet.size();j++)
      {
        TreeSet<Integer> toStates=new TreeSet<Integer>();
        for(int k=0;k<dfa.DFAMoveFunctions.size();k++)
        {
          MoveFunction funct=dfa.DFAMoveFunctions.get(k);
          if(funct.from.equals(thisState)&&funct.index==j)
          {
            int l=0;
            for(EClosure e:dfa.stateList)
            {
              if(e.equals(funct.to))
              {
                toStates.add(l);
                break;
              }
              l++;
            }
          }
        }
        stateDestinations.add(toStates);
      }
      dfa.transitionTable.add(stateDestinations);
    }
    int p=0;
    for(EClosure o:dfa.stateList)
    {
      if(o.equals(originalStartState))
      {
        dfa.startingState=p;
      }
      p++;
    }
    return dfa;
  }
}
