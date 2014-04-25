/* Decision Tree User Interface
 * Author: Thomas Lyons
 * Completed March 6, 2014 at 4:58am
 *
 * This program provides the user with a primitive command command-line GUI to
 * test the capabilities of a decision tree class and it's accompanying
 * manipulator classes. */
#include"DecisionTree.h"
#include"Printer.h"
#include"Trainer.h"
#include"Classifier.h"
#include"Report.h"
#include<iostream>
#include<sstream>
//Print the menu options
void printOptions(bool treeExists);
//Read in decision tree metadata and training files and create a decision tree
DecisionTree*readInUserDefinedFilesAndCreateTree(bool&treeExists);
/* Read in a file of unclassified instances and classify them using the decision
 * tree */
void classifyInstances(DecisionTree*tree);
//Print the decision tree
void printTree(DecisionTree*tree);
//Test how well the decision tree classifies a series of instances
void reportAccuracy(DecisionTree*tree);
//Exit the program
void printQuit();
//Print an error
void printError();
int main()
{
  DecisionTree*tree=NULL;
  bool treeExists=false;
  string inputString="";
  //Print the title
  cout<<"    +------------------------------------------------+"<<endl;
  cout<<"    |              Decision Tree Tester              |"<<endl;
  cout<<"    +------------------------------------------------+"<<endl;
  cout<<"    |                By Thomas Lyons                 |"<<endl;
  cout<<"    +------------------------------------------------+"<<endl;
  cout<<"    |            Please select an option             |"<<endl;
  while(true)
  {
    //Print the menu options
    printOptions(treeExists);
    cin.sync();
    //Get a user menu choice
    getline(cin,inputString);
    istringstream userInputStream(inputString);
    int inputNumber;
    if(userInputStream>>inputNumber)
    {
      //If no tree has been created, only let the user create a tree
      if(!treeExists)
      {
        switch(inputNumber)
        {
          case(1):
            tree=readInUserDefinedFilesAndCreateTree(treeExists);
            break;
          case(2):
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
        switch(inputNumber)
        {
          case(1):
            delete tree;
            tree=readInUserDefinedFilesAndCreateTree(treeExists);
            break;
          case(2):
            classifyInstances(tree);
            break;
          case(3):
            printTree(tree);
            break;
          case(4):
            reportAccuracy(tree);
            break;
          case(5):
            printQuit();
            delete tree;
            tree=NULL;
            return 0;
            break;
          default:
            printError();
            break;
        }
      }
    }
    //Input error
    else printError();
  }
}
void printOptions(bool treeExists)
{
  cout<<"+---+------------------------------------------------+"<<endl;
  cout<<"| 1 | Create a tree from metadata and training files |"<<endl;
  if(!treeExists)
  {
    cout<<"+---+------------------------------------------------+"<<endl;
    cout<<"| 2 | Quit the program.                              |"<<endl;
    cout<<"+---+------------------------------------------------+"<<endl;
  }
  else
  {
    cout<<"| 2 | Classify instances and print to file           |"<<endl;
    cout<<"| 3 | Print out the decision tree                    |"<<endl;
    cout<<"| 4 | Report on the accuracy of a tree               |"<<endl;
    cout<<"| 5 | Quit the program.                              |"<<endl;
    cout<<"+---+------------------------------------------------+"<<endl;
  }
  cout<<"Enter choice: ";
}
DecisionTree* readInUserDefinedFilesAndCreateTree(bool&treeExists)
{
  //Prompt the user for file names
  string metaDataFileName;
  cout<<"Please enter the metadata file name: ";
  getline(cin,metaDataFileName);
  string trainingDataFileName;
  cout<<"Please enter the training data file name: ";
  getline(cin,trainingDataFileName);
  //Open the files
  ifstream metaStream(metaDataFileName.c_str());
  ifstream trainStream(trainingDataFileName.c_str());
  //Create a decision tree from the files
  DecisionTree*tree=new DecisionTree();
  if(metaStream.is_open()&&trainStream.is_open())
  {
    Trainer*trainer=new Trainer(metaStream,trainStream,tree);
    cout<<"Tree created"<<endl;
    //A tree exists now
    treeExists=true;
    //Clean up
    delete trainer;
    trainer=NULL;
    metaStream.close();
    trainStream.close();
    return tree;
  }
  else
  {
    cout<<"Error reading files"<<endl;
    delete tree;
    tree=NULL;
    treeExists=false;
    return NULL;
  }
}
void classifyInstances(DecisionTree*tree)
{
  //Prompt the user for file names
  string instanceDataFileName;
  cout<<"Please enter the name of the file containing the"<<endl;
  cout<<"instances you would like to classify: ";
  getline(cin,instanceDataFileName);
  string outputFileName;
  cout<<"Please enter the name of the file you would like to"<<endl;
  cout<<"write to: ";
  getline(cin,outputFileName);
  //Open the files
  ifstream iStream(instanceDataFileName);
  ofstream oStream(outputFileName);
  if(iStream.is_open()&&oStream.is_open())
  {
    //Classify the instances
    Classifier*classifier=new Classifier(iStream,tree,oStream);
    cout<<"Instances classified and printed to file"<<endl;
    //Clean up
    delete classifier;
    classifier=NULL;
    iStream.close();
    oStream.close();
  }
  else
  {
    cout<<"Error opening files"<<endl;
  }
}
void printTree(DecisionTree*tree)
{
  //Print the tree
  Printer*printer=new Printer(tree,cout);
  //Clean up
  delete printer;
  printer=NULL;
}
void reportAccuracy(DecisionTree*tree)
{
  //Prompt the user for a file name
  string testInstanceDataFileName;
  cout<<"Please enter the name of the file containing the test"<<endl;
  cout<<"instances you would like to test against the tree: ";
  getline(cin,testInstanceDataFileName);
  //Open the file
  ifstream inputFile(testInstanceDataFileName);
  if(inputFile.is_open())
  {
    //Report on the accuracy of the tree
    Report*report=new Report(inputFile,tree,cout);
    //Clean up
    delete report;
    report=NULL;
    inputFile.close();
  }
  else
  {
    cout<<"Error opening file"<<endl;
  }
}
void printQuit()
{
  cout<<"Exiting..."<<endl;
}
void printError()
{
  cout<<"Sorry, the input you entered could not be parsed."<<endl;
  cout<<"Please try again."<<endl;
}
