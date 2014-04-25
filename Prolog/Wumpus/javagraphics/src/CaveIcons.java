import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;

import javax.swing.ImageIcon;
import javax.swing.JFrame;


public class CaveIcons implements ImageObserver {
	
	private ImageIcon[] icons = new ImageIcon[10];
	private int tileWidth, tileHeight;
	
	public static final int PIT = 0;
	public static final int LIVING_WUMPUS = 1;
	public static final int DEAD_WUMPUS = 2;
	public static final int ARROW = 3;
	public static final int AGENT_W_ARROW = 4;
	public static final int GOLD = 7;
	public static final int CAVE_FLOOR = 8;
	public static final int PLAY_BUTTON = 9;
	
	
	public CaveIcons()
	{
		tileWidth = 50;
		tileHeight = 50;
	
		icons[PIT] = new ImageIcon("./images/spikedpit.jpg");
		icons[LIVING_WUMPUS] = new ImageIcon("./images/wumpus.gif");
		//need dead wumpus picture
		icons[DEAD_WUMPUS] = new ImageIcon("./images/wumpus.gif");
		icons[ARROW] = new ImageIcon("./images/arrow.jpg");
		icons[AGENT_W_ARROW] = new ImageIcon("./images/green_arrow.jpg");
		
		//Living Agent with no-Arrow
		icons[5] = new ImageIcon("./images/green_arrow.jpg");
		
		//Dead Agent
		icons[6] = new ImageIcon("./images/green_arrow.jpg");
		
		
		icons[GOLD] = new ImageIcon("./images/gold.jpg");
		icons[CAVE_FLOOR] = new ImageIcon("./images/lightcavefloor.jpg");
		
		//controls
		icons[PLAY_BUTTON] = new ImageIcon("./images/play_button.jpg");
		//wall
		
		
		
		for (int i=0; i<9; i++)
		{
			
			this.resizeTile(i);
			if (i == 4)
			{
				this.rotateIcon(4,-90);
			}
		}
		
		this.sizeControls();

	}
	
	
	public ImageIcon getTile(int index)
	{
		return icons[index];
	}
	
	public void setTileSize(int height, int width)
	{
		this.tileHeight = height;
		this.tileWidth = width;
	}
	
	
	
	
	public int getTileWidth() {
		return tileWidth;
	}


	public int getTileHeight() {
		return tileHeight;
	}


	public void resizeTile(int index)
	{
		
		Image img = icons[index].getImage();		
		if (index != CaveIcons.GOLD)
		{
		img = img.getScaledInstance(tileHeight, tileWidth, Image.SCALE_REPLICATE);
		}
		else
		{
		img = img.getScaledInstance((int)(tileHeight/2), (int)(tileWidth/2), Image.SCALE_REPLICATE);
		}
		icons[index].setImage(img);
		
		
		//System.out.println("?: ");
	}
	
	/**
	 * Sets the image of an CaveIcon.
	 * @param index - Value for the type of icon.
	 * @param image - Path to the image.
	 * @return - Returns true if the image was able to load successfully.
	 */
	public boolean setTileImage(int index, String image)
	{
		icons[index] = new ImageIcon(image);
		int status = icons[index].getImageLoadStatus();
		if (status != MediaTracker.COMPLETE)
		{
			return false;
		}
		this.resizeTile(index);
		return true;
	}
	
	private void sizeControls()
	{
		Image img;
		img = icons[PLAY_BUTTON].getImage();		
		img = img.getScaledInstance((int)(img.getHeight(this)*.5),(int)( img.getWidth(this)*.5), Image.SCALE_SMOOTH);
		icons[PLAY_BUTTON].setImage(img);
		
	}

	public void rotateIcon(int IconIndex,double degrees)
	{
		
		Image img = icons[IconIndex].getImage();
		/*
		MediaTracker mt = new MediaTracker(container);
		    mt.addImage(img, 0);
		    try {
		      mt.waitForID(0);
		    } catch (InterruptedException ie) {
		 }
		*/
		
		BufferedImage bi = new BufferedImage(img.getWidth(this),img.getWidth(this), BufferedImage.TYPE_4BYTE_ABGR);
		
		
		//img = (BufferedImage)icons[IconIndex].getImage();
		Graphics2D g2 = (Graphics2D) bi.getGraphics();
	    g2.rotate(degrees * Math.PI / 180, tileWidth/2, tileHeight /2 );
		g2.drawImage(img, 0, 0, null);
		icons[IconIndex].setImage(bi);
		
	}

	@Override
	public boolean imageUpdate(Image arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5) {
		// TODO Auto-generated method stub
		return false;
	}
		

}
