Thomas Lyons
IT340, Spring 2014
Illinois State University
Program 2

TO COMPILE:
Place all of the headers, source files, and the provided Makefile in the same
folder and use one of the following commands:

1. make release       --> compiles a release version of the code
2. make debug         --> compiles a debug version of the code
3. make clean         --> removes all .o files for a clean compilation
4. make clean release --> combines option 1 and 3
5. make clean debug   --> combines option 2 and 3

Please be aware that this program is intended for the c++11 standard so I
sincerely hope you are compiling this on the correct machine.

TO RUN:
From the same folder you compiled in, type

./program2

and execute.
Be sure to provide correct paths for your input and output files.

OPTION 1:
This option will allow the user to read in a meta data file and a training
file. The program will parse the files and create a decision tree trainer
from the result. In milestone 2, this will be more useful, but for now you
can't do much with the user-defined files because the creation of use-defined
trees has not yet been implemented.

OPTION 2:
This option will allow the user to input a file containing instances and allow
the user to specify a file to output to. Right now, dummy data from
weather.meta and weather.train as well as a dummy test file weather.test are
used. In milestone 2, user-defined data will be used.

OPTION 3:
This option prints out the generated decision tree. Right now it only prints
out a dummy tree because user-defined trees have not yet been implemented.

OPTION 4:
This option reports on the accuracy of the generated decision tree. This has
not yet been implemented yet.

OPTION 5:
This option exits the program.
