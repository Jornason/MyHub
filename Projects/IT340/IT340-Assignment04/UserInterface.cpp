//Naive Bayes Classifier User Interface
//Author: Thomas Lyons
//Completed April 15, 2014 at 1:58am

//This program provides the user with a primitive command command-line GUI to
//test the capabilities of a Naive Bayes Classifier

#include"ProbabilityTable.h"
#include"Printer.h"
#include"Trainer.h"
#include"Classifier.h"
#include"Report.h"
#include<iostream>
#include<sstream>
//Print the menu options
void printOptions(bool tableExists);
//Read in decision tree metadata and training files and create a decision tree
ProbabilityTable* readInUserDefinedFilesAndCreateTable(bool& tableExists);
//Read in a file of unclassified instances and classify them using the
//probability table
void classifyInstances(ProbabilityTable* table);
//Print the probability table
void printTable(ProbabilityTable* table);
//Test how well the probability table classifies a series of instances
void reportAccuracy(ProbabilityTable* table);
//Exit the program
void printQuit();
//Print an error
void printError();

int main()
{
  ProbabilityTable* table = NULL;
  bool tableExists = false;
  string inputString = "";
  //Print the title
  cout << "    +------------------------------------------------+" << endl;
  cout << "    |             Naive Bayes Classifer              |" << endl;
  cout << "    +------------------------------------------------+" << endl;
  cout << "    |                By Thomas Lyons                 |" << endl;
  cout << "    +------------------------------------------------+" << endl;
  cout << "    |            Please select an option             |" << endl;
  while (true)
  {
    //Print the menu options
    printOptions(tableExists);
    cin.sync();
    //Get a user menu choice
    getline(cin, inputString);
    istringstream userInputStream(inputString);
    int inputNumber;
    if (userInputStream >> inputNumber)
    {
      //If no table has been created, only let the user create a table
      if (!tableExists)
      {
        switch (inputNumber)
        {
          case (1):
            table = readInUserDefinedFilesAndCreateTable(tableExists);
            break;
          case (2):
            printQuit();
            return 0;
            break;
          default:
            printError();
            break;
        }
      }
      //Give the user more choices
      else
      {
        switch (inputNumber)
        {
          case (1):
            delete table;
            table = readInUserDefinedFilesAndCreateTable(tableExists);
            break;
          case (2):
            classifyInstances(table);
            break;
          case (3):
            printTable(table);
            break;
          case (4):
            reportAccuracy(table);
            break;
          case (5):
            printQuit();
            delete table;
            table = NULL;
            return 0;
            break;
          default:
            printError();
            break;
        }
      }
    }
    //Input error
    else
    {
      printError();
    }
  }
}

void printOptions(bool tableExists)
{
  cout << "+---+-------------------------------------------------+" << endl;
  cout << "| 1 | Create a table from metadata and training files |" << endl;
  if (!tableExists)
  {
    cout << "+---+-------------------------------------------------+" << endl;
    cout << "| 2 | Quit the program.                               |" << endl;
    cout << "+---+-------------------------------------------------+" << endl;
  }
  else
  {
    cout << "| 2 | Classify instances and print to file            |" << endl;
    cout << "| 3 | Print out the probability table                 |" << endl;
    cout << "| 4 | Report on the accuracy of a table               |" << endl;
    cout << "| 5 | Quit the program.                               |" << endl;
    cout << "+---+-------------------------------------------------+" << endl;
  }
  cout << "Enter choice: ";
}

ProbabilityTable* readInUserDefinedFilesAndCreateTable(bool& tableExists)
{
  //Prompt the user for file names
  string metaDataFileName;
  cout << "Please enter the metadata file name: ";
  getline(cin, metaDataFileName);
  string trainingDataFileName;
  cout << "Please enter the training data file name: ";
  getline(cin, trainingDataFileName);
  //Open the files
  ifstream metaStream(metaDataFileName.c_str());
  ifstream trainStream(trainingDataFileName.c_str());
  //Create a probability table from the files
  ProbabilityTable* table = new ProbabilityTable();
  if (metaStream.is_open() && trainStream.is_open())
  {
    Trainer* trainer = new Trainer(metaStream, trainStream, table);
    cout << "Table created" << endl;
    //A table exists now
    tableExists = true;
    //Clean up
    delete trainer;
    trainer = NULL;
    metaStream.close();
    trainStream.close();
    return table;
  }
  else
  {
    cout << "Error reading files" << endl;
    delete table;
    table = NULL;
    tableExists = false;
    return NULL;
  }
}

void classifyInstances(ProbabilityTable* table)
{
  //Prompt the user for file names
  string instanceDataFileName;
  cout << "Please enter the name of the file containing the" << endl;
  cout << "instances you would like to classify: ";
  getline(cin, instanceDataFileName);
  string outputFileName;
  cout << "Please enter the name of the file you would like to" << endl;
  cout << "write to: ";
  getline(cin, outputFileName);
  //Open the files
  ifstream iStream(instanceDataFileName);
  ofstream oStream(outputFileName);
  if (iStream.is_open() && oStream.is_open())
  {
    //Classify the instances
    Classifier* classifier = new Classifier(iStream, table, oStream);
    cout << "Instances classified and printed to file" << endl;
    //Clean up
    delete classifier;
    classifier = NULL;
    iStream.close();
    oStream.close();
  }
  else
  {
    cout << "Error opening files" << endl;
  }
}

void printTable(ProbabilityTable* table)
{
  //Print the table
  Printer* printer = new Printer(table, cout);
  //Clean up
  delete printer;
  printer = NULL;
}

void reportAccuracy(ProbabilityTable* table)
{
  //Prompt the user for a file name
  string testInstanceDataFileName;
  cout << "Please enter the name of the file containing the test" << endl;
  cout << "instances you would like to test against the tree: ";
  getline(cin, testInstanceDataFileName);
  //Open the file
  ifstream inputFile(testInstanceDataFileName);
  if (inputFile.is_open())
  {
    //Report on the accuracy of the table
    Report* report = new Report(inputFile, table, cout);
    //Clean up
    delete report;
    report = NULL;
    inputFile.close();
  }
  else
  {
    cout << "Error opening file" << endl;
  }
}

void printQuit()
{
  cout << "Exiting..." << endl;
}

void printError()
{
  cout << "Sorry, the input you entered could not be parsed." << endl;
  cout << "Please try again." << endl;
}
