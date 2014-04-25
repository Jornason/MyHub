import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.StringTokenizer;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import jpl.Query;


public class WumpusGUI extends JFrame implements ActionListener{
	

        public static void main (String [] args)
	
	{
		
		//load the necessary files
		Query q1 = new Query("consult('navigate.pl')");
		q1.hasSolution();

		String t1 = "consult('swi_wumpus.pl')";
		q1 = new Query(t1);
		q1.hasSolution();
					
		//start the gui from prolog.
		t1 = "start_gui";
		q1 = new Query(t1);
		q1.hasSolution();
						
	}


	protected static JTabbedPane jtp;
	protected JButton submit;
	protected JTextField path;
	public static Cave thecave;
	
	
	
	private JTextField[] fields;
	
	public WumpusGUI(){
		super("Wumpus World");
		thecave = new Cave(600,600);
		this.setSize(800,800);
		Container contents = getContentPane();
		jtp = new JTabbedPane();
		jtp.addTab("Settings", this.settings());
		jtp.addTab("The Cave", thecave);
		jtp.addTab("Map Maker", new JCheckBox("This is Tab Three"));
		contents.add(jtp);
		
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.pack();
		this.setVisible(true);
		
	}
	
	
	public static void constructNewCave()
	{
	   thecave.reset();
	   
	}
	
	
	private JPanel settings()
	{
	   	
		String labels [] = {"Agent Path","Number of Trials"};
		int widths[] = {60,10};
		JPanel settingsPanel = new JPanel();
		JPanel labelPanel = new JPanel(new GridLayout(labels.length,1));
		JPanel fieldPanel = new JPanel(new GridLayout(labels.length,1));
		settingsPanel.add(labelPanel);
		settingsPanel.add(fieldPanel);
		fields = new JTextField[labels.length];
		
		for (int i=0; i < labels.length; i++)
		{
			fields[i] = new JFormattedTextField();
			fields[i].addPropertyChangeListener("value",new FormattedTextFieldListener());
	
			if (i < widths.length)
			 fields[i].setColumns(widths[i]);
			
			JLabel lab = new JLabel(labels[i],JLabel.RIGHT);
			lab.setLabelFor(fields[i]);
			labelPanel.add(lab);
			JPanel p = new JPanel(new FlowLayout(FlowLayout.LEFT));
			p.add(fields[i]);
			fieldPanel.add(p);
		}
		
		
		submit = new JButton("Submit");
		submit.setActionCommand("submitForm");
		submit.addActionListener(this);
		
		
	    settingsPanel.add(submit);
		return settingsPanel;
		
		/*
		JPanel panel = new JPanel();
		panel.setLayout(new FlowLayout());
		JPanel agentPath = new JPanel();
		path = new JTextField("",60);
		agentPath.add(path);
		path.setActionCommand("chaged");
		path.addActionListener(this);
		agentPath.setBorder(new TitledBorder("Path to your agent"));
		agentPath.setBounds(0,0,100,50);
		 add(Box.createRigidArea(new Dimension(0, 10)));
		panel.add(agentPath);
		 add(Box.createRigidArea(new Dimension(0, 10)));
	    consult = new JButton("Consult Agent");
		consult.setBounds(100, 90, 100, 50);
		//consult.setMnemonic(KeyEvent.VK_D);
		consult.setActionCommand("consult");
		consult.addActionListener(this);
		panel.add(consult);
		
		

		/*
		panel.add(agentPath);
		 add(Box.createRigidArea(new Dimension(0, 10)));
		
		
		
		JPanel trials = new JPanel();
		JTextField tr = new JTextField("",6);
		trials.add(tr);
		tr.setActionCommand("chaged");
		tr.addActionListener(this);
		trials.setBorder(new TitledBorder("Path to your agent"));
		trials.setBounds(0,0,100,50);
		 add(Box.createRigidArea(new Dimension(0, 10)));
		panel.add(tr);
		*/
		//return panel;
		
		
	}
	


	@Override
	public void actionPerformed(ActionEvent e) {
				
		if ("submitForm".equals(e.getActionCommand()))
		{
			//consult agent
			System.out.println("Action: " + e.getActionCommand());
			
			//evaluate for set number of trials
			String t = fields[1].getText();
			//int trials = Integer.getInteger(T);
			String agent = fields[0].getText();

			
			if (DisplayManager.isWindows())
			{
			System.out.println("\\");
		  	
			StringTokenizer st = new StringTokenizer(agent,"\\");
			
			String file = "";
		    while (st.hasMoreTokens())
		    {
		    	file += st.nextToken() + "\\\\";
		    	
		    }
		    
		    file = file.substring(0, (file.length()-2));
            agent = file;
			}
			Query q1 = new Query("consult('"+ agent +"')");
			q1.hasSolution();
			
			
			
			submit.setEnabled(false);
			
			Thread thr = new Thread(new Evaluate(t));
		    thr.start();
			
			
			
		}
	}

	private  class FormattedTextFieldListener implements PropertyChangeListener {

		@Override
		public void propertyChange(PropertyChangeEvent evt) {
			{
				submit.setEnabled(true);
			}
			
		}
	

		
	       
	    

	
	}
}
