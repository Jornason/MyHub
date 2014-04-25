This program solves problems using various algorithms

The various algorithms and problems now have their own mains. Pick from the following:

BreadthFirstFWGC
DepthFirstFWGC
BreadthFirstEightPuzzle
DepthFirstEightPuzzle
ManhattanEightPuzzle
NumDisplacedEightPuzzle

To compile, the most expediant way is to run the following command in terminal in the project
folder

javac *.java

For all of these commands, the command syntax is as follows

<PROGRAM_NAME> [NUM_ARRAY] [DEBUG_OPTION]

To specify a number array for the eight puzzle starting point, simply list the numbers in order,
starting from the upper left-hand corner and moving towards the lower right-hand corner. The
blank space can be represented as "0" or "9". The puzzle

+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 7 | 8 |   |
+---+---+---+

can be represented as

1 2 3 4 5 6 7 8 0

Here are some valid commands to get your started:

//Run the Breadth First algorithm on a Farmer-Wolf-Goat-Cabbage Problem with debugging turned on
java BreadthFirstFWGC debug

//Run the Depth First algorithm on a Farmer-Wolf-Goat-Cabbage Problem with debugging turned on
java DepthFirstFWGC debug

//Run the Breadth First algorithm on a Farmer-Wolf-Goat-Cabbage Problem with debugging turned off
java BreadthFirstFWGC

//Run the Depth First algorithm on a Farmer-Wolf-Goat-Cabbage Problem with debugging turned off
java DepthFirstFWGC

//Run the Breadth First algorithm on an Eight Puzzle Problem with debugging turned on
java BreadthFirstEightPuzzle 0 8 7 6 5 4 3 2 1 debug

//Run the Depth First algorithm on an Eight Puzzle Problem with debugging turned on
java DepthFirstEightPuzzle 0 8 7 6 5 4 3 2 1 debug

//Run the Breadth First algorithm on an Eight Puzzle Problem with debugging turned off
java BreadthFirstEightPuzzle 0 8 7 6 5 4 3 2 1

//Run the Depth First algorithm on an Eight Puzzle Problem with debugging turned off
java DepthFirstEightPuzzle 0 8 7 6 5 4 3 2 1

//Run the Manhattan Heuristic A-Star Search on an Eight Puzzle Problem with debugging turned on
java ManhattanEightPuzzle 0 7 8 5 6 4 2 3 1 debug

//Run the Number of tiles displaced algorithm on an Eight Puzzle Problem with debugging turned on
java NumDisplacedEightPuzzle 0 1 8 2 7 3 6 4 5 debug

//Run the Manhattan Heuristic A-Star Search on an Eight Puzzle Problem with debugging turned off
java ManhattanEightPuzzle 0 7 8 5 6 4 2 3 1

//Run the Number of tiles displaced algorithm on an Eight Puzzle Problem with debugging turned off
java NumDisplacedEightPuzzle 0 1 8 2 7 3 6 4 5

You may find it necessary to tack on various runtime flags to give the program the necessary
resources to finish. I often found that I could run the program with the following flags

java -Xmx256m -Xss256m [PROGRAM_AND_ARGUMENTS_HERE]

Your results may vary depending on the machine resources and the problem being solved.
