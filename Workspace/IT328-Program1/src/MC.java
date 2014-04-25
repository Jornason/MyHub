// Maximum Clique Algorithm
// By Thomas Lyons

import java.util.ArrayList;
import java.util.Arrays;
public class MC
{
 boolean[][]graph;
 int nV;
 long timeStart;
 long timeEnd;
 int maxSize;
 boolean[]solution;
 MC(int numVertices,boolean[][]graph)
 {
  this.nV=numVertices;
  this.graph=graph;
  maxSize=0;
  solution=new boolean[numVertices];
 }
 void findMaxClique()
 {
  int i;
//Start the clock.
  timeStart=System.currentTimeMillis();
//The current clique being tested.
  ArrayList<Integer>clique=new ArrayList<Integer>();
//The candidates to maybe add to the clique.
  ArrayList<Integer>cnd=new ArrayList<Integer>(nV);
//All vertices are candidates to start with.
  for(i=0;i<nV;i++)
  {
   cnd.add(i);
  }
//Increase the size of the clique.
  grow(clique,cnd);
//Stop the clock.
  timeEnd=System.currentTimeMillis();
 }
 void grow(ArrayList<Integer>clq,ArrayList<Integer>cnd)
 {
//Test each candidate, starting with the last one.
  for(int i=cnd.size()-1;i>=0;i--)
  {
//Is the clique at a maximum possible size?
   if(clq.size()+cnd.size()<=maxSize)
   {
    return;
   }
//Add a candidate vertex to the clique.
   int vertexToAdd=cnd.get(i);
   clq.add(vertexToAdd);
//Find the new candidates.
   ArrayList<Integer>newCnd=new ArrayList<Integer>();
   for(int j=0;j<=i;j++)
   {
//From the old candidates, get a vertex.
    int vertexToCompare=cnd.get(j);
//If the vertex added to the clique and the candidate
//vertex connect, add it to the new candidates
    if(graph[vertexToAdd][vertexToCompare])
    {
     newCnd.add(vertexToCompare);
    }
   }
//If there are no new candidates and clique is at a
//new max size, save the solution.
   if(newCnd.isEmpty()&&clq.size()>maxSize)
   {
    saveSolution(clq);
   }
//If there are new candidates, grow the clique.
   if(!newCnd.isEmpty())
   {
    grow(clq,newCnd);
   }
//Otherwise, the vertex is useless.
   clq.remove(clq.size()-1);
   cnd.remove(i);
  }
 }
 void saveSolution(ArrayList<Integer>clique)
 {
 //Save the clique for printing later.
  Arrays.fill(solution,false);
  for(int i=0;i<clique.size();i++)
  {
   solution[clique.get(i)]=true;
  }
 //Update the maximum size.
  maxSize=clique.size();
 }
}
