// Thomas Lyons
// IT328 Spring 2014
// Program 2
// DFA Move Function
// This class represents a move function in a DFA. It includes the state that
// the
// DFA is moving from, the index of the alphabet entry that triggers the
// transition, and the state that the DFA transitions to.
public class MoveFunction
{
  // The EClosure in the DFA States set that this move function applies to.
  EClosure from;
  // An index in the alphabet list that this move function applies to
  int      index;
  // The EClosure in the DFA states set that this move function moves the
  // machine's current state to.
  EClosure to;
  // Constructor
  public MoveFunction(EClosure fromState,int index,EClosure toState)
  {
    from=fromState;
    this.index=index;
    to=toState;
  }
  // Overridden toString (useful for Eclipse Debugger)
  @Override public String toString()
  {
    StringBuilder sb=new StringBuilder();
    sb.append(from.toString());
    sb.append("--");
    sb.append(index);
    sb.append("-->");
    sb.append(to.toString());
    return sb.toString();
  }
}
