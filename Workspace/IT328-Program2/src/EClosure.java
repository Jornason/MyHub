import java.util.Iterator;
import java.util.TreeSet;
// Thomas Lyons
// IT328 Spring 2014
// Program 2
// E-Closure Class
// This class represents an E-Closure. That is, a set of states that can be
// reached from a given state using a member of the alphabet. The E-Closure can
// be marked to show that it has already been dealt with in the process of
// conversion from NFA to DFA.
public class EClosure implements Comparable<EClosure>
{
  // If true, we can ignore this E closure when looking for places to start from
  public boolean          marked;
  // The states that can be reached from the this state
  public TreeSet<Integer> states;
  // Default constructor
  public EClosure()
  {
    // Not marked to begin with
    marked=false;
    states=new TreeSet<Integer>();
  }
  // Overridden compareTo method
  @Override public int compareTo(EClosure that)
  {
    // We don't care if it is marked. Only the content of the states
    if(states.size()==that.states.size())
    {
      Iterator<Integer> thisIter=states.iterator();
      Iterator<Integer> thatIter=that.states.iterator();
      while(thisIter.hasNext())
      {
        int thisI=thisIter.next();
        int thatI=thatIter.next();
        if(thisI>thatI)
        {
          return 1;
        }else if(thisI<thatI){ return -1; }
      }
      return 0;
    }else if(states.size()>that.states.size())
    {
      return 1;
    }else
    {
      return -1;
    }
  }
  // Overridden equals method
  @Override public boolean equals(Object obj)
  {
    // We don't care if it is marked, only the content of the states
    if(obj==null){ return false; }
    if(getClass()!=obj.getClass()){ return false; }
    final EClosure that=(EClosure)obj;
    if(states.size()!=that.states.size()){ return false; }
    Iterator<Integer> iter1=states.iterator();
    Iterator<Integer> iter2=that.states.iterator();
    while(iter1.hasNext())
    {
      int i=iter1.next();
      int j=iter2.next();
      if(i!=j){ return false; }
    }
    return true;
  }
  // Overridden toSTring method (useful for Eclipse debugger)
  @Override public String toString()
  {
    StringBuilder sb=new StringBuilder();
    Iterator<Integer> it=states.iterator();
    sb.append("[");
    if(it.hasNext())
    {
      sb.append(it.next());
    }
    while(it.hasNext())
    {
      sb.append(" ");
      sb.append(it.next());
    }
    sb.append("]");
    return sb.toString();
  }
}
