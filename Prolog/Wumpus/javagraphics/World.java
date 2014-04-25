/*
 * Filename: World.java
 * Author: Shawn Rhoney
 * Date: October, 2011
 * Project: Wumpus World GUI
 */

import jpl.*;

/**
 * The World class initializes the Wumpus World.
 * @author Shawn Rhoney
 *
 */
public class World {
		
	
	public static void helloworld()
{
	System.out.println("hello world");
}
	public static void main (String [] args)
	
	{
		
		//load the necessary files
		Query q1 = new Query("consult('navigate.pl')");
		q1.hasSolution();

		String t1 = "consult('swi_wumpus.pl')";
		q1 = new Query(t1);
		q1.hasSolution();
					
		//start the gui from prolog.
		//t1 = "start_gui";
		//q1 = new Query(t1);
		//q1.hasSolution();
						
	}
		
}

