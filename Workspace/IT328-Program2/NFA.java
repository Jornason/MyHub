import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;
import java.util.TreeSet;
// Thomas Lyons
// IT328 Spring 2014
// Program 2
// Non-deterministic Finite Automata Class
// This class represents an NFA It has each state, it's transitions, an
// acceptable alphabet, the starting state, and final, states.
public class NFA
{
  // Find all of the states that can be reached from the given parent states
  // using an alphabet symbol (index)
  public static TreeSet<Integer>
      buildEClosure(TreeSet<Integer> referencingStates,
          TreeSet<Integer> parentStates,
          int index,
          NFA nfa)
  {
    // This is the set that will be returned by this call
    TreeSet<Integer> returnSet=new TreeSet<Integer>();
    // Go through each of the states in the parentState set
    for(Integer i:parentStates)
    {
      // Don't get caught in an infinite loop by continually adding a state to
      // the set and then going to that state and repeating the process
      if(index==nfa.alphabet.size())
      {
        if(referencingStates.contains(i))
        {
          returnSet.add(i);
          continue;
        }
      }
      // Get the set of states that can be reached from this state by using the
      // alphabet entry at the index as well as any e transitions
      TreeSet<Integer> transitions=nfa.transitionTable.get(i).get(index);
      // If we are doing E-transitions, ensure no infinite looping occurs by
      // keeping track of where we have been
      if(index==nfa.alphabet.size())
      {
        referencingStates.add(i);
      }
      // Find all the states that can be reached from THOSE states by
      // recursively
      // calling the function (using E as the alphabet entry)
      TreeSet<Integer> newStates=buildEClosure(referencingStates,
          transitions,
          nfa.alphabet.size(),
          nfa);
      // Add each of the resulting states to the return set
      for(Integer j:newStates)
      {
        returnSet.add(j);
      }
    }
    // We should be returning a set of states from the NFA that can be reached
    // using the alphabet entry from the set of states that we started with.
    // This
    // will include any E-transitions where the alphabet entry could be
    // anything.
    return returnSet;
  }
  // The number of states in this finite state machine
  public int                                    numberOfStates;
  // The starting state
  public int                                    startingState;
  // The set of final states
  public TreeSet<Integer>                       finalStates;
  // The set of acceptable characters in the string
  public TreeSet<String>                        alphabet;
  // The table of states and transitions
  public ArrayList<ArrayList<TreeSet<Integer>>> transitionTable;
  // Default constructor
  public NFA()
  {
    numberOfStates=0;
    startingState=0;
    finalStates=new TreeSet<Integer>();
    alphabet=new TreeSet<String>();
    transitionTable=new ArrayList<ArrayList<TreeSet<Integer>>>();
  }
  // Copy constructor
  public NFA(NFA fsm)
  {
    alphabet=fsm.alphabet;
    startingState=fsm.startingState;
    // Create a new transition table. Leave it empty for now
    transitionTable=new ArrayList<ArrayList<TreeSet<Integer>>>();
    // Declare the final states
    finalStates=new TreeSet<Integer>();
  }
  // Construct from a file
  public NFA(String filename)
  {
    File infile=new File(filename);
    try
    {
      Scanner sc=new Scanner(infile);
      // First line is the number of states in the finite state machine
      String nextLine=sc.nextLine();
      numberOfStates=Integer.parseInt(nextLine);
      // Second line is the acceptable characters in the alphabet
      alphabet=new TreeSet<String>();
      nextLine=sc.nextLine();
      StringTokenizer st=new StringTokenizer(nextLine);
      while(st.hasMoreElements())
      {
        alphabet.add((String)st.nextElement());
      }
      // The next lines are the states and their transitions
      transitionTable=new ArrayList<ArrayList<TreeSet<Integer>>>();
      int i=0;
      while(i<numberOfStates)
      {
        nextLine=sc.nextLine();
        // Delimit based on the opening bracket of the transition
        st=new StringTokenizer(nextLine,"{");
        // Ignore the first part of the line
        @SuppressWarnings("unused") String garbageStr=(String)st.nextElement();
        ArrayList<TreeSet<Integer>> stateTran=new ArrayList<TreeSet<Integer>>();
        int j=0;
        while(st.hasMoreElements())
        {
          TreeSet<Integer> transition=new TreeSet<Integer>();
          String transitionString=(String)st.nextElement();
          Scanner stateScanner=new Scanner(transitionString);
          // Ignore non-numeric characters
          stateScanner.useDelimiter("[^0-9]+");
          while(stateScanner.hasNextInt())
          {
            transition.add(stateScanner.nextInt());
          }
          // The e-transitions are always self-referencing
          if(j==alphabet.size())
          {
            transition.add(i);
          }
          stateScanner.close();
          stateTran.add(transition);
          j++;
        }
        transitionTable.add(stateTran);
        i++;
      }
      // Get the starting state
      nextLine=sc.nextLine();
      startingState=Integer.parseInt(nextLine);
      // Get the final states
      finalStates=new TreeSet<Integer>();
      nextLine=sc.nextLine();
      Scanner finalStateScanner=new Scanner(nextLine);
      // Ignore non-numeric characters
      finalStateScanner.useDelimiter("[^0-9]+");
      while(finalStateScanner.hasNextInt())
      {
        finalStates.add(finalStateScanner.nextInt());
      }
      finalStateScanner.close();
      sc.close();
    }catch(FileNotFoundException e)
    {
      System.err.println("Failed to read finite state machine file.");
      e.printStackTrace();
    }
  }
}
