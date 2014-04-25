/*
 * Filename: Evaluate.java
 * Author: Shawn Rhoney
 * Date: November, 2011
 * Project: Wumpus World GUI
 */
 
/**
 * The Evaluate class starts a series of trials.
 */
public class Evaluate implements Runnable {

	private String trials;
	
	
   /**
    * Constructor
    * @param trials - A String which is the value for the number of trials.
    */
   public Evaluate(String trials)
   {
	   this.trials = trials;
   }


	@Override
	public void run() {
		DisplayManager.evaluate(trials);
		
	}
   
}
