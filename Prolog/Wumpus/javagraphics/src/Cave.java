import java.awt.Dimension;

import java.awt.GridLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;


import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.Timer;

import jpl.Query;


public class Cave extends JLayeredPane implements ActionListener{
	
	private CaveIcons icons;
	private HashMap<Point,JLabel> gold;
	private JLabel arrow;
	private char agentType;
	private char wumpusType;
	private JLabel agent;
	private int agentOrientation;
	private Timer clock;
	
	protected JButton nextAction;
	protected JTextField numTrials;
	protected JLabel score;
	protected JLabel action;
	protected JLabel numMoves;
	
	public boolean getAction;
	
    private  int moves;	
	
	
	/*
       Layer 0 - Cave Floor
       Layer 1 - Pits
       Layer 2 - Wumpus
       Layer 3 - Arrow
       Layer 4 - Agent
       Layer 5 - Gold
	 */ 
 	private static final int numLayers = 6;
	
 	private int offset;
 	
	
	public Cave(int width, int height){
		
        moves = 0;
		gold = new HashMap<Point,JLabel>();
		this.agentOrientation = 0;
		wumpusType = 'L';
		agentType = 'A';
		getAction = false;
		icons = new CaveIcons();
		offset = icons.getTileWidth() + 10;
		Point origin = new Point(30,30);
		this.setPreferredSize(new Dimension(width,height));
		
		//this.setBorder(call to--->custom wall border)
		
		//Draws the bottom layer of the cave.
		for (int j = 0; j<8; j++)
        {
        
        	origin.x = 30;
        	for (int i = 0; i < 8; i++) {
        		JLabel label = new JLabel(icons.getTile(CaveIcons.CAVE_FLOOR));
        		this.addJLabel(label, origin, new Integer(0));
        		origin.x += offset;
            
        	}
        	origin.y += offset;
        }
		
		this.addControlPanel();
		this.updateScore();
	}
		
	public int getAgentOrientation() {
		return agentOrientation;
	}

	public void setAgentOrientation(int agentOrientation) {
		this.agentOrientation = agentOrientation;
	}

	public void drawPits(Point[] pitLocations)
	{
		for (int i=0; i < pitLocations.length; i++)
		{
			JLabel label = new JLabel(icons.getTile(CaveIcons.PIT));
			int x = ((pitLocations[i].x-1) * offset) + 30;
			int y = (8 * offset) -(pitLocations[i].y *offset) + 30;
			this.addJLabel(label, new Point(x,y), new Integer(1));
			
		}
	}
	
	public void drawGold(Point[] goldLocations)
	{
		for (int i=0; i < goldLocations.length; i++)
		{
			
			JLabel label = new JLabel(icons.getTile(CaveIcons.GOLD));
			int x = ((goldLocations[i].x-1) * offset) + 30;
			int y = (8 * offset) -(goldLocations[i].y *offset) + 30;
			this.addJLabel(label, new Point(x,y), new Integer(5));
			gold.put(goldLocations[i],label);
		}
	}
	
	public void drawAgent(Point location, int orientation)
	{
		if (agent != null)
		{
			this.remove(agent);
			this.repaint();
		}
		
		agent = new JLabel();
		/*
		if (this.agentOrientation != orientation && this.agentOrientation != 0)
		{
			if (orientation < this.agentOrientation)
			{
				icons.rotateIcon(CaveIcons.AGENT_W_ARROW, 90);	
			}
			else
			{
				icons.rotateIcon(CaveIcons.AGENT_W_ARROW, -90);
			}
			this.agentOrientation = orientation;
		}
		*/
		if (this.agentOrientation != orientation )
		{
			int rotate = this.agentOrientation - orientation;
			
			//if (orientation == 90)
			//{
				icons.rotateIcon(CaveIcons.AGENT_W_ARROW, rotate);
			//}
			//if (orientation == 270)
			//{
				//icons.rotateIcon(CaveIcons.AGENT_W_ARROW, 90);
			//}
			this.agentOrientation = orientation;
		}
	
	
		switch(agentType)
		{
			case 'A': agent.setIcon(icons.getTile(CaveIcons.AGENT_W_ARROW));
		         break;
		}
		
		int x = ((location.x-1) * offset) + 30;
		int y = (8 * offset) -(location.y *offset) + 30;

		this.addJLabel(agent, new Point(x,y), new Integer(4));
		
	}
	
	public void drawWumpus(Point location)
	{
		JLabel label = new JLabel();
	
		switch(wumpusType)
		{
			case 'L': label = new JLabel(icons.getTile(CaveIcons.LIVING_WUMPUS));
		         break;
			case 'D': label = new JLabel(icons.getTile(CaveIcons.DEAD_WUMPUS));
				break;
		}
		
		int x = ((location.x-1) * offset) + 30;
		int y = (8 * offset) -(location.y *offset) + 30;
		this.addJLabel(label, new Point(x,y), new Integer(2));
		
	}
	
	
	public void grabGold(Point aPoint)
	{
		JLabel pickup = gold.get(aPoint);
		this.remove(pickup);
		this.repaint();
	}
	
	
	private void addJLabel(JLabel label, Point origin, Integer layer){
		//label.setOpaque(true);
		label.setBounds(origin.x,origin.y, icons.getTileWidth(), icons.getTileHeight());
		this.add(label, layer);
	}
	
	private void addControlPanel()
	{
		JPanel panel = new JPanel();
		panel.setLayout(new GridLayout());
		nextAction = new JButton("Next Action",icons.getTile(CaveIcons.PLAY_BUTTON));
		nextAction.setActionCommand("performAction");
		nextAction.addActionListener(this);
		//nextAction.setBounds(10, 0, 0, 75);
		panel.add(nextAction);
		
		action = new JLabel();
		action.setBorder(BorderFactory.createTitledBorder("Action"));
		panel.add(action);
		
		JPanel trials = new JPanel();
		trials.setBorder(BorderFactory.createTitledBorder("Trials"));
		numTrials = new JTextField("1",8);
		trials.add(numTrials);
		panel.add(trials);
		
		numMoves = new JLabel();
		numMoves.setBorder(BorderFactory.createTitledBorder("Moves"));
		panel.add(numMoves);
		
		
		
		score = new JLabel();
		score.setBorder(BorderFactory.createTitledBorder("Score"));
		panel.add(score);
		
		panel.setBounds(30,offset*9, offset*8,100);
		this.add(panel);
	}

	public void exitCave()
	{
		this.remove(agent);
		this.repaint();
		//add  wait statement
		this.moves = 0;
		
	}
	
	public void fireArrow()
	{
		// java swing timer to control the animation
		clock = new Timer(50, this);
		clock.start();
		clock.setCoalesce(false);
		System.out.println("Firing Arrow!");
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		
		
	  if (e.getSource().getClass().toString().equals("class javax.swing.Timer"))
		{
			this.animateArrow();
			nextAction.setEnabled(false);
			
			if (arrow == null)
			{
				clock.stop();
				nextAction.setEnabled(true);
			}
			
		}
		
		if ("performAction".equals(e.getActionCommand()))
			{
			    if(numTrials.isEnabled())
			    {
			    	//System.out.println(numTrials.getText());
			    	//Query q1 = new Query("evaluate_agent(" + numTrials.getText()  +",S,T)");
					//q1.hasSolution();
					 //DisplayManager.eval();
				   //this.repaint();
				  numTrials.setEnabled(false);
			    }
			
			    this.getAction = true;
			    
			    //String query = "clock(A)";
			    //Query t = new Query(query);
			    //String time = (String) t.oneSolution().get("A");
			    
			    
			   //String query = "run_agent_trial("+DisplayManager.getAPT("1") +" )";
			   //Query actions = new Query(query);
			   //actions.hasSolution();
			}

	}

	private void animateArrow() {
		
		Point from = new Point(agent.getX(),agent.getY());
		
		if (arrow != null)
		{
			from = new Point(arrow.getX(),arrow.getY());
			this.remove(arrow);
			this.repaint();
		}
		arrow = new JLabel(icons.getTile(CaveIcons.ARROW));
	
		
		boolean endsAtZero = true;
		boolean xaxis = true;
		int movement = (int)(icons.getTileHeight()*.25);
		
		switch (agentOrientation)
		{
		case 0: endsAtZero = false;
		        xaxis = true;
		        break;
		case 90: endsAtZero = false;
		         xaxis = false;
		         break;
		case 180: endsAtZero = true;
		          xaxis = true;
		          movement = movement * -1;
		          break;
		case 270: endsAtZero = true;
		          xaxis = false;
		          movement = movement * -1;
		          break;
		}
		
		
		boolean wallhit = false;
		// Stop clock when the arrow has hit a wall.
		if (endsAtZero && xaxis && from.x == 30)
		{
		    wallhit = true;
			this.remove(arrow);
			this.repaint();
		    arrow = null;
		}
		if (!endsAtZero && xaxis && from.x == offset*7+30)
		{
			
			wallhit = true;
			this.remove(arrow);
			this.repaint();
			arrow = null;
		}
		if (endsAtZero && !xaxis && from.y == offset*7+30)
		{
			wallhit = true;
            this.remove(arrow);
            this.repaint();
		    arrow = null;
		}
		if (!endsAtZero && !xaxis && from.y == 30)
		{
			wallhit = true;
			this.remove(arrow);
			this.repaint();
		    arrow = null;
		}
		if (!wallhit)
		{
			int x = from.x;
			int y = from.y;
			if(xaxis)
			{
			   x = x + movement;	
			}
			else
			{
			   y = y + movement;	
			}
			
			this.addJLabel(arrow, new Point(x,y), new Integer(6));
		}
		
	}

	public void reset() {
		this.removeAll();
		gold = new HashMap<Point,JLabel>();
		this.agentOrientation = 0;
		wumpusType = 'L';
		agentType = 'A';
		getAction = false;
		icons = new CaveIcons();
		offset = icons.getTileWidth() + 10;
		Point origin = new Point(30,30);
		
		//Draws the bottom layer of the cave.
		for (int j = 0; j<8; j++)
        {
        
        	origin.x = 30;
        	for (int i = 0; i < 8; i++) {
        		JLabel label = new JLabel(icons.getTile(CaveIcons.CAVE_FLOOR));
        		this.addJLabel(label, origin, new Integer(0));
        		origin.x += offset;
            
        	}
        	origin.y += offset;
        }
		
		this.addControlPanel();
		
		
	}
	
	public void updateMoveCounter()
	{
		
		String m = moves + "";
		this.numMoves.setText(m);
		this.moves++;
	}
	
	public void updateScore()
	{
		String s = DisplayManager.getScore() + "";
		score.setText(s);
		
	}

}
