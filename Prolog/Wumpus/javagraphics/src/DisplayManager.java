import java.awt.Point;

import jpl.Query;




/**
 * @author Shawn Rhoney
 *
 */
public class DisplayManager {
	
    //no longer need
	public static String [][] cave = new String [8][8];
	
	private static double time;
	private static int numactions;
	private static String [] percepts;
	private static int score = 0;
	
	
	public static Point[] setPoints(int [] Xs, int [] Ys)
	{
		Point[] points = new Point [Xs.length];
		for (int i=0; i<Xs.length; i++)
		{
			System.out.println(Xs[i] + " " + Ys[i]);
			points[i] = new Point(Xs[i],Ys[i]);
		}
		
		return points;
	}
    
    
	public static void setPits(Point [] pits)
	{
		System.out.println("Num Pits: "+ pits.length);
	}
	
	public static void setPits(int [] xCords, int [] yCords)
	{
		for (int i=0; i<xCords.length; i++)
		{
			if (cave[xCords[i]-1][yCords[i]-1] == null)
				cave[xCords[i]-1][yCords[i]-1]= "P";
			else
				cave[xCords[i]-1][yCords[i]-1] += "P";
		}
	}
	
	public static void setGold(int [] xCords, int [] yCords)
	{
		for (int i=0; i<xCords.length; i++)
		{
			if (cave[xCords[i]-1][yCords[i]-1] == null)
				cave[xCords[i]-1][yCords[i]-1]= "G";
			else
				cave[xCords[i]-1][yCords[i]-1] += "G";
		}
	}
	
	public static void setWumpus(int [] wumpusLocation)
	{
		if (cave[wumpusLocation[0]-1][wumpusLocation[1]-1] == null)
		cave[wumpusLocation[0]-1][wumpusLocation[1]-1] = "W";
		else
		cave[wumpusLocation[0]-1][wumpusLocation[1]-1] += "W";
		
		
		
	}
	
	public static void setStatus(String [] status1, int [] status2)
	{
		System.out.println("-----------------");
		
		System.out.println("Incave: " + status1[0]);
		System.out.println("Alive: " + status1[1]);
		System.out.println("Has Gold: " + status2[0]);
		System.out.println("Has Arrow: " + status2[1]);
			
	}
	
	public static void setScore(int scr)
	{
		score = scr;
	}
	
	public static void setPercepts(String [] percepts)
	{
		System.out.println("here");
		//Stench,Breeze,Glitter,Bump,Scream,
		System.out.println("Stench: " + percepts[0] + "Breeze: " + percepts[1] + "Glitter: " + percepts[2] +
				"Bump: " + percepts[3] + "Scream: " + percepts[4]);
	}
	
	
	
	public static void setAgentOrientation(int orientation)
	{
		System.out.println("Degree agent is facing: " + orientation);
	}
	
	
	public static void setAgentLocation(int X, int Y)
	{
		if (cave[X-1][Y-1] == null)
			cave[X-1][Y-1] = "A";
		else
			cave[X-1][Y-1] += "A";
	}
	
	public static void agentPerforms(String action)
	{
		System.out.print("----> ");
		System.out.println(action);
	}
	
	public static void printCave()
	{
		for(int i=7; i>=0; i--){
			for (int j=0; j<8; j++)
			{
				if (cave[j][i] == null)
				  System.out.printf("%10s","|"); 
				  else
				System.out.printf("%10s", cave[j][i] + "|");
			}
		System.out.println();
		}
	}
	
	public static void delay(int numactions, String [] percepts)
	{
		System.out.println("-------------->assinging data<---------------");
		DisplayManager.numactions = numactions;
		DisplayManager.percepts = percepts;
		
	}
	
	
	public static void test(String [] t)
	{
		System.out.println("testing");
		for (int i=0; i < t.length ; i++)
		{
			System.out.println(t[i]);
		}
	}
	
	
	public static void evaluate(String trials)
	{
		String t1 = "evaluate_agent(" +trials+ ",S,T)";
		Query q1 = new Query(t1);
		q1.hasSolution();
	}


	
	 private static String OS = null;
	  private static String getOsName()
	   {
	      if(OS == null) { OS = System.getProperty("os.name"); }
	      return OS;
	   }
	   public static boolean isWindows()
	   {
		   return getOsName().startsWith("Windows");
	   }




	
	
	public static String getAPT(String time) {
	
		String perc = ",[";
		for (int i=0; i<DisplayManager.percepts.length; i++)
		{
		   perc += DisplayManager.percepts[i];
		   perc += ",";
		}
		perc += "_,_],";
		
		return "" + DisplayManager.numactions + perc + time;
	}


	public static int getScore() {
		// TODO Auto-generated method stub
		return score;
	}
	
}
