import java.util.ArrayList;
import java.util.Iterator;
import java.util.TreeSet;
// Thomas Lyons
// IT328 Spring 2014
// Program 2
// Deterministic Finite Automata Class
// This class represents an DFA It has each state, it's transitions, an
// acceptable alphabet, the starting state, final states, move functions, and
// list of valid states.
public class DFA extends NFA
{
  // The set of states in the DFA
  public TreeSet<EClosure> stateList;
  // Move functions map DFA states to other DFA states through a transitions
  // using a symbol in the alphabet (EClosure -> X -> EClosure)
  ArrayList<MoveFunction>  DFAMoveFunctions;
  // Construct a DFA from an NFA txt file
  public DFA(String filename)
  {
    DFAMoveFunctions=new ArrayList<MoveFunction>();
    // First create an NFA from the file
    NFA nfa=new NFA(filename);
    // The DFA will share the NFA's alphabet
    alphabet=nfa.alphabet;
    stateList=new TreeSet<EClosure>();
    // The number of unmarked states in the set of DFA states
    int numUnmarked;
    // To begin with, find all of the states we can get to from the starting
    // state using only "E transitions". This will be the first state in the DFA
    EClosure sZero=new EClosure();
    TreeSet<Integer> zeroState=new TreeSet<Integer>();
    zeroState.add(nfa.startingState);
    // The referencing states ensure that we never loop back infinitely while
    // adding E-transitions to the DFA state
    TreeSet<Integer> refStates=new TreeSet<Integer>();
    // Recursively build the starting state
    sZero.states=buildEClosure(refStates,zeroState,nfa.alphabet.size(),nfa);
    stateList.add(sZero);
    numUnmarked=1;
    // Continue until no more unmarked states exist
    while(numUnmarked>0)
    {
      // Find an unmarked state
      EClosure T=new EClosure();
      for(EClosure e:stateList)
      {
        if(e.marked==false)
        {
          // Mark the state and use it to find more states
          numUnmarked--;
          e.marked=true;
          T=e;
          break;
        }
      }
      // See where we can get to from this state using all of the symbols
      int i=0;
      while(i<nfa.alphabet.size())
      {
        EClosure s=new EClosure();
        TreeSet<Integer> emptySet=new TreeSet<Integer>();
        // Call the recursive function to add states
        s.states=buildEClosure(emptySet,T.states,i,nfa);
        // If this is a new state with a size greater than 0 add it to the DFA
        // states as an unmarked state
        if(!stateList.contains(s)&&s.states.size()>0)
        {
          stateList.add(s);
          numUnmarked++;
        }
        // If the DFA state was of size greater than 0, map the move function
        if(s.states.size()>0)
        {
          MoveFunction newFunct=new MoveFunction(T,i,s);
          DFAMoveFunctions.add(newFunct);
        }
        i++;
      }
    }
    // Add accepting states to the DFA
    Iterator<EClosure> it=stateList.iterator();
    int i=0;
    while(i<stateList.size())
    {
      EClosure e=it.next();
      for(Integer j:e.states)
      {
        // A DFA state is a final state if it contains a state in the NFA that
        // was also a final state
        if(nfa.finalStates.contains(j))
        {
          finalStates.add(i);
        }
      }
      i++;
    }
    // Get the starting state for the DFA
    Iterator<EClosure> it2=stateList.iterator();
    i=0;
    while(i<stateList.size())
    {
      EClosure e=it2.next();
      // The DFA's starting state is equal to the first state we added in the
      // process
      if(e.equals(sZero))
      {
        startingState=i;
        break;
      }
      i++;
    }
    // Set the number of states
    numberOfStates=stateList.size();
    // The transition table is used for convenient printing. It's basically the
    // 2D array you see printed out
    transitionTable=new ArrayList<ArrayList<TreeSet<Integer>>>();
    for(i=0;i<numberOfStates;i++)
    {
      ArrayList<TreeSet<Integer>> stateDests=new ArrayList<TreeSet<Integer>>();
      int m=0;
      EClosure thisState=new EClosure();
      // This is the state that we are traveling from
      for(EClosure state:stateList)
      {
        if(i==m)
        {
          thisState=state;
          break;
        }
        m++;
      }
      // Consider all of the symbols in the alphabet
      for(int j=0;j<alphabet.size();j++)
      {
        TreeSet<Integer> toStates=new TreeSet<Integer>();
        // These are the states that we can get to from this state using the
        // alphabet symbol
        for(int k=0;k<DFAMoveFunctions.size();k++)
        {
          MoveFunction funct=DFAMoveFunctions.get(k);
          if(funct.from.equals(thisState)&&funct.index==j)
          {
            int l=0;
            for(EClosure e:stateList)
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
        stateDests.add(toStates);
      }
      transitionTable.add(stateDests);
    }
  }
}
