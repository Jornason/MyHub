/*
 * 3SSAT -> Adjacency Matrix program
 * 
 * copyright Ben Murray
 * 
 */
import java.io.*;

public class Convert3SAT {
	
	public static void main(String[] args){
		
		int [][] adjacencies = null;
		
		if (args.length >= 1 && args[0] != null){
			BufferedReader br = null;
			PrintWriter pw = null;
			try {
				br = new BufferedReader(new FileReader(args[0]));
				pw = new PrintWriter("graphscustom.txt", "UTF-8");

				String currentLine;
				
				while ((currentLine = br.readLine()) != null) {
					/* Break line into values, then convert to integer values */
					String[] stringEntries = currentLine.split(" ", 0);
					int[] entries = new int[stringEntries.length-1];
					int value = Integer.parseInt(stringEntries[0]);
					for (int i = 1; i < stringEntries.length; i++){
						entries[i-1] = Integer.parseInt(stringEntries[i]);
					}
					
					/* Initialize values to check against */
					int[] values = new int[value*2];
					int counter = 1;
					for (int i=0; i<values.length; i++){
						values[i] = counter;
						if (i%2 == 0)
							counter *= -1;
						else
							counter = (-counter)+1;
					}
					
					/* Create adjacency table, then fill it based on clauses 
					 * (table is symmetric and only needs to be iterated over half).
					 */
					adjacencies = new int[value*2][value*2];
					for (int i =  0; i < adjacencies.length; i++){			// current row
						for (int j = i; j < adjacencies.length; j++){		// current column
							
							/*
							 * Check if the being compared against yourself 
							 * or your negation. If neither, check if your 
							 * compared value exists elsewhere within the 3CNF. 
							 */
							int determination = 0;
							
							// If matching with self, set adjacenct
							if (i == j){
								//System.out.println("Entry (" + i + ", " + j + ") is reflexive\n");
								determination = 1;
							}
							
							// If matching with negation, set non-adjacent
							else if (j == i+1 && j%2==1){
								//System.out.println("Entry (" + i + ", " + j + ") is a negation\n");
								determination = 0;
							}
							else{
								//System.out.println("Checking entry (" + i + ", " + j + ")");

								// Go through 3CNF checking clauses
								for (int k = 0; k < entries.length; k++){
									
									// If current value (ith) exists in 3CNF check for checked value (jth)
									if (entries[k] == values[i]){
										//System.out.println("Value " + values[i] + " exists at position " + k);
										
										// Examine entries for matches to checked value (jth) 
										// outside current clause
										int prevClauseEnd = k - k%3;
										int nextClauseBeg = k + (3-k%3);
										//System.out.println("Checking for " + values[j] + " excluding between positions " + prevClauseEnd + " and " + nextClauseBeg);
										for (int l = 0; l < entries.length; l++){
											if (entries[l] == values[j]){
												//System.out.println("Found " + values[j] + " at position " + l + "\n");
												determination = 1;
												l = entries.length;
											}
											else
												determination = 0;
											// If about to enter own clause, move to next one
											if (l == prevClauseEnd)
												l = nextClauseBeg-1;
										}
										//if (determination == 0)
											//System.out.println("Couldn't find " + values[j] + " in entries\n");
									}
									if (determination == 1)
										k = entries.length;
									
									// Otherwise, variable doesn't exist in the 3CNF
									//if (k == entries.length && determination == 0)
										//System.out.println("Couldn't find " + values[i] + " in entries\n");

								}
							}
							
							adjacencies[i][j] = determination;
							adjacencies[j][i] = determination;
						}
					}
					
					/*
					 * For testing print out adjacency matrix
					 * Output to graphscustom.txt
					 */
					pw.println(value*2);
					for (int i = 0; i < adjacencies.length; i++){
						for (int j = 0; j < adjacencies[i].length; j++){
							int data = adjacencies[i][j];
							if (j > 0){
								pw.print(" " + data);
							}
							else{
								pw.print(data);
							}
						}
						pw.println();
					}
				}
				pw.close();
			} catch (IOException e){
				e.printStackTrace();
			} finally{
				try {
					if (br != null)
						br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		
		else{
			System.out.println("Please specify a file as a command-line argument.");
		}
	}
	
}
