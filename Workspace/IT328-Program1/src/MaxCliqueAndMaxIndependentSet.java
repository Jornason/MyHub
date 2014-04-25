// Maximum Clique and Maximum Independent Set Tester
// By Thomas Lyons

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
public class MaxCliqueAndMaxIndependentSet
{
	//Shorter print invocations.
	public static void pl(String str)
	{
		System.out.println(str);
	}
	public static void p(String str)
	{
		System.out.print(str);
	}
	public static void main(String[]args)
	{
		//Go through the 3 files
		pl("Testing graphs.txt");
		readFile("graphs.txt");
		pl("Testing graphsDense.txt");
		readFile("graphsDense.txt");
		pl("Testing graphscustom.txt");
		readFile("graphscustom.txt");
	}
	public static void readFile(String filename)
	{
		//Open the file and read it into the graph.
		File infile=new File(filename);
		try
		{
			Scanner sc=new Scanner(infile);
			//Indicates which graph is being tested.
			int nG=0;
			while(sc.hasNextInt())
			{
				//Get the number of vertices.
				int nV=sc.nextInt();
				//If there are no vertices, exit.
				if(nV==0)
				{
					sc.close();
					return;
				}
				//Create an adjacency matrix and it's complement.
				boolean[][]graph=new boolean[nV][nV];
				boolean[][]complementGraph=new boolean[nV][nV];
				int i,j;
				//Read in vertices
				for(i=0;i<nV;i++)
				{
					for(j=0;j<nV;j++)
					{
						int num=sc.nextInt();
						if(i==j)
						{
							//Never allow a vertex to connect to itself (it will crash).
							graph[i][j]=false;
							complementGraph[i][j]=false;
						}
						//There is a connection.
						else if(num!=0)
						{
							graph[i][j]=true;
							//The complement graph is the opposite
							complementGraph[i][j]=false;
						}
						//There is not a connection.
						else
						{
							graph[i][j]=false;
							//The complement graph is the opposite.
							complementGraph[i][j]=true;
						}
					}
				}
				//Count the number of edges
				int nE=0;
				for(i=0;i<nV;i++)
				{
					for(j=i+1;j<nV;j++)
					{
						if(graph[i][j]==true)
						{
							nE++;
						}
					}
				}
				nG++;
				//Find the max clique.
				MC mc=new MC(nV,graph);
				mc.findMaxClique();
				//Print the max clique.
				p("Max Clique          G"+nG+" ("+nV+", "+(nE+nV)+") {");
				//Create a printable solution and read in vertices to it.
				ArrayList<Integer>sol=new ArrayList<Integer>(mc.maxSize);
				for(i=0;i<nV;i++)
				{
					if(mc.solution[i]==true)
					{
						sol.add(i);
					}
				}
				p(""+sol.get(0));
				for(i=1;i<sol.size();i++)
				{
					p(","+sol.get(i));
				}
				pl("} (size="+mc.maxSize+", "+(mc.timeEnd-mc.timeStart)+"ms)");
				//Find the max independent set.
				MC mis=new MC(nV,complementGraph);
				mis.findMaxClique();
				//Create a printable solution and read in vertices.
				ArrayList<Integer>sol2=new ArrayList<Integer>(mis.maxSize);
				//If there are no edges.
				if(nE==0)
				{
					p("Max Independent Set G"+nG+" ("+nV+", "+(nE+nV)+") {");

					for(i=0;i<nV;i++)
					{
						if(mis.solution[i]==true)
						{
							sol2.add(i);
						}
					}
					p(""+sol2.get(0));
					pl("} (size="+1+", "+(mis.timeEnd-mis.timeStart)+"ms)");
				}
				//If there are edges.
				else
				{
					p("Max Independent Set G"+nG+" ("+nV+", "+(nE+nV)+") {");
					for(i=0;i<nV;i++)
					{
						if(mis.solution[i]==true)
						{
							sol2.add(i);
						}
					}
					p(""+sol2.get(0));
					for(i=1;i<sol2.size();i++)
					{
						p(","+sol2.get(i));
					}
					pl("} (size="+mis.maxSize+", "+(mis.timeEnd-mis.timeStart)+"ms)");
				}
				//Print the 3CNF.
				p("3-CNF             No."+nG+" (n="+nV+", "+(nE+nV)+")\nAssignments:[");
				//Create a printable solution and read in vertices to it.
				p("A"+sol.get(0)+"=");
				if(sol.get(0)%2 == 0)
					p("T");
				else
					p("F");
				for(i=1;i<sol.size();i++)
				{
					p(", A"+sol.get(i));
					if(sol.get(i)%2 == 0)
						p("=T");
					else
						p("=F");
				}
				pl("] (" + (mc.timeEnd-mc.timeStart)+"ms)");
			}
			sc.close();
		}
		catch(FileNotFoundException e)
		{
			e.printStackTrace();
		}
	}
}
