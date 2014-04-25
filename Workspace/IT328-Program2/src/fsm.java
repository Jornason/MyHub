import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;
import java.util.TreeSet;
// Thomas Lyons
// IT328 Spring 2014
// Program 2
// FSM Driver class
// This class converts an NFA to a DFA, a minimized DFA and determines if a
// series of string conform to the language.
public class fsm
{
  public static void main(String[] args)
  {
    // The Non-deterministic finite automata
    NFA nfa;
    // The non-minimized deterministic finite automata
    DFA dfaNonMin;
    // The minimized deterministic finite automata
    DFA dfaMin;
    // The list of strings to be processed
    ArrayList<ArrayList<String>> inputStrings;
    // Read in the NFA
    nfa=new NFA(args[0]);
    // Create a non-minimized DFA
    dfaNonMin=new DFA(args[0]);
    // Read in the input strings
    inputStrings=parseInputStrings(args[1],nfa.alphabet);
    // Output the results
    printNFA(nfa);
    System.out.print("To DFA:");
    printDFAStates(dfaNonMin);
    printDFA(dfaNonMin);
    runFiniteStateMachine(dfaNonMin,inputStrings);
    // Create a minimized DFA
    dfaMin=MinimizeDFA.minimizeDFA(dfaNonMin);
    // Output the results
    System.out.print("\nMinimized DFA: ");
    printDFAStates(dfaMin);
    printDFA(dfaMin);
    runFiniteStateMachine(dfaMin,inputStrings);
  }
  // Parse the input strings and create usable input for the DFA
  private static ArrayList<ArrayList<String>>
      parseInputStrings(String filename,TreeSet<String> alphabet)
  {
    File infile=new File(filename);
    // This is the item we will return
    ArrayList<ArrayList<String>> returnList=new ArrayList<ArrayList<String>>();
    try
    {
      Scanner sc=new Scanner(infile);
      while(sc.hasNextLine())
      {
        String nextLine=sc.nextLine();
        if(!nextLine.isEmpty())
        {
          // Each line is a sentence
          ArrayList<String> sentence=new ArrayList<String>();
          // Each character is a symbol
          int i=0;
          String symbol="";
          while(i<nextLine.length())
          {
            symbol+=nextLine.charAt(i);
            sentence.add(symbol);
            symbol="";
            i++;
          }
          returnList.add(sentence);
        }
      }
      sc.close();
    }catch(FileNotFoundException e)
    {
      System.err.println("Failed to read input string file.");
      e.printStackTrace();
    }
    return returnList;
  }
  // The string was accepted
  private static void printAccepted(ArrayList<String> sentence)
  {
    printSentence(sentence);
    System.out.println(" is accepted");
  }
  // Print a DFA
  private static void printDFA(DFA dfa)
  {
    // Print the alphabet
    System.out.print(" Sigma:");
    int i,j;
    int length=6;
    for(String a:dfa.alphabet)
    {
      System.out.print("     "+a);
      length+=5;
      length+=a.length();
    }
    // Print a dividing line
    System.out.println();
    System.out.print(" ");
    for(i=0;i<length;i++)
    {
      System.out.print("-");
    }
    System.out.println();
    // Print each state
    for(i=0;i<dfa.numberOfStates;i++)
    {
      // Keep everything aligned
      if(i<10)
      {
        System.out.print("     "+i+":");
      }else if(i<100)
      {
        System.out.print("    "+i+":");
      }else if(i<1000)
      {
        System.out.print("   "+i+":");
      }else if(i<10000)
      {
        System.out.print("  "+i+":");
      }else
      {
        System.out.print(" "+i+":");
      }
      // Handle each alphabet entry
      for(j=0;j<dfa.alphabet.size();j++)
      {
        TreeSet<Integer> transition=dfa.transitionTable.get(i).get(j);
        if(transition.size()==0)
        {
          System.out.print("      ");
          continue;
        }
        int toState=transition.first();
        // Keep everything aligned
        if(toState<10)
        {
          System.out.print("     "+toState);
        }else if(toState<100)
        {
          System.out.print("    "+toState);
        }else if(toState<1000)
        {
          System.out.print("   "+toState);
        }else if(toState<10000)
        {
          System.out.print("  "+toState);
        }else
        {
          System.out.print(" "+toState);
        }
      }
      System.out.println();
    }
    // Print the dividing line
    System.out.print(" ");
    for(i=0;i<length;i++)
    {
      System.out.print("-");
    }
    System.out.println();
    // Print the starting state
    System.out.println("s:  "+dfa.startingState);
    // Print the list of final states
    System.out.print("A: {");
    Iterator<Integer> it=dfa.finalStates.iterator();
    while(it.hasNext())
    {
      int finalState=it.next();
      System.out.print(finalState);
      if(it.hasNext())
      {
        System.out.print(",");
      }
    }
    System.out.println("}");
  }
  // Print the states in the DFA//
  private static void printDFAStates(DFA dfa)
  {
    for(EClosure e:dfa.stateList)
    {
      System.out.print("{");
      Iterator<Integer> it=e.states.iterator();
      while(it.hasNext())
      {
        System.out.print(it.next());
        if(it.hasNext())
        {
          System.out.print(",");
        }
      }
      System.out.print("}");
    }
    System.out.println();
  }
  // Print an NFA
  private static void printNFA(NFA nfa)
  {
    // Print the alphabet
    System.out.print("Sigma:");
    int i,j;
    int length=6;
    for(String a:nfa.alphabet)
    {
      System.out.print(a+" ");
      length+=a.length()+1;
    }
    length-=1;
    // Print the dividing line
    System.out.println();
    for(i=0;i<length;i++)
    {
      System.out.print("-");
    }
    System.out.println();
    // Print each state
    for(i=0;i<nfa.numberOfStates;i++)
    {
      System.out.print(i+":  ");
      Iterator<String> it=nfa.alphabet.iterator();
      j=0;
      // Print for each alphabet entry
      while(it.hasNext())
      {
        String alphabetEntry=it.next();
        System.out.print("("+alphabetEntry+",{");
        Iterator<Integer> it2=nfa.transitionTable.get(i).get(j).iterator();
        while(it2.hasNext())
        {
          int toState=it2.next();
          System.out.print(toState);
          if(it2.hasNext())
          {
            System.out.print(",");
          }
        }
        System.out.print("}) ");
        j++;
      }
      // Print the E-transitions
      System.out.print("( ,{");
      if(nfa.transitionTable.get(i).get(j).size()==1)
      {
        System.out.println("})");
        continue;
      }
      Iterator<Integer> it3=nfa.transitionTable.get(i).get(j).iterator();
      int toState=it3.next();
      while(toState==i)
      {
        toState=it3.next();
      }
      System.out.print(toState);
      while(it3.hasNext())
      {
        toState=it3.next();
        if(toState!=i)
        {
          System.out.print(","+toState);
        }
      }
      System.out.print("})");
      System.out.println();
    }
    // Print the dividing line
    for(i=0;i<length;i++)
    {
      System.out.print("-");
    }
    System.out.println();
    // Print the starting state
    System.out.println("s:  "+nfa.startingState);
    // Print the final states
    System.out.print("A: {");
    Iterator<Integer> it3=nfa.finalStates.iterator();
    while(it3.hasNext())
    {
      int finalState=it3.next();
      System.out.print(finalState);
      if(it3.hasNext())
      {
        System.out.print(",");
      }
    }
    System.out.println("}");
    System.out.println();
  }
  // The string was rejected
  private static void printRejected(ArrayList<String> sentence)
  {
    printSentence(sentence);
    System.out.println(" is rejected");
  }
  // Print the string
  private static void printSentence(ArrayList<String> sentence)
  {
    for(int i=0;i<sentence.size();i++)
    {
      System.out.print(sentence.get(i));
    }
  }
  // Find if a string is accepted or rejected
  private static void runFiniteStateMachine(DFA dfa,
      ArrayList<ArrayList<String>> inputStrings)
  {
    int i,j,k;
    // Go through each sentence
    for(i=0;i<inputStrings.size();i++)
    {
      // The sentence being tested
      ArrayList<String> sentence=inputStrings.get(i);
      // Always start at the starting state
      int currentState=dfa.startingState;
      // We have not failed yet
      boolean failed=false;
      for(j=0;j<sentence.size();j++)
      {
        // Go through each symbol in the sentence and traverse the DFA
        String symbol=sentence.get(j);
        Iterator<String> it=dfa.alphabet.iterator();
        k=0;
        while(it.hasNext())
        {
          // Find if the symbol is contained in the alphabet
          String alphabetSymbol=it.next();
          if(alphabetSymbol.equals(symbol))
          {
            break;
          }
          k++;
        }
        // The symbol is not valid
        if(k==dfa.alphabet.size())
        {
          printRejected(sentence);
          break;
        }
        // The transition is not valid
        else if(dfa.transitionTable.get(currentState).get(k).isEmpty())
        {
          printRejected(sentence);
          break;
        }else
        {
          // Perform the transition
          currentState=dfa.transitionTable.get(currentState).get(k).first();
        }
      }
      if(failed)
      {
        // The sentence failed. Go to the next sentence
        continue;
      }
      if(!dfa.finalStates.contains(currentState))
      {
        // The sentence did not end properly
        printRejected(sentence);
      }else
      {
        // Sentence is correct
        printAccepted(sentence);
      }
    }
  }
}
