#include <stdio.h>
#include <list>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
  vector<string> fileNameList;
  fileNameList.push_back("Box.cpp");
  fileNameList.push_back("Cone.cpp");
  fileNameList.push_back("Cylinder.cpp");
  fileNameList.push_back("main.cpp");
  fileNameList.push_back("mesh.cpp");
  fileNameList.push_back("Plane.cpp");
  fileNameList.push_back("Sphere.cpp");
  fileNameList.push_back("View.cpp");
  fileNameList.push_back("Box.h");
  fileNameList.push_back("Cone.h");
  fileNameList.push_back("Cylinder.h");
  fileNameList.push_back("mesh.h");
  fileNameList.push_back("Object.h");
  fileNameList.push_back("ObjectXMLReader.h");
  fileNameList.push_back("Plane.h");
  fileNameList.push_back("Sphere.h");
  fileNameList.push_back("View.h");
  for(int i = 0; i < fileNameList.size(); i++)
  {
    list<char> characters;
    char c;
    FILE* pFile;
    string original = "C:\\Users\\Thomas\\Dropbox\\Programming\\C++\\QTFiles\\Projects\\IT287\\IT287-ResearchProject\\"+fileNameList[i];
    pFile = fopen(original.c_str(), "r");
    if(pFile != NULL)
    {
      do{
        c=fgetc(pFile);
        printf("%c", c);
        characters.push_back(c);
      }while(c!= EOF);
      fclose(pFile);
      cout<<"File closed"<<endl;
      //while(*characters.begin() == ' ')
      //{
      //  characters.erase(character.begin());
      //  cout<<"space removed"<<endl;
      //}
      /* clean bound comments */
      list<char>::iterator beginBoundComment;
      list<char>::iterator endBoundComment;
      list<char>::iterator beginLineComment;
      list<char>::iterator endLineComment;
      bool inBoundComment=false;
      bool inLineComment=false;
      bool inDoubleQuote=false;
      bool inSingleQuote=false;
      list<char>::iterator curr;
      for(curr=characters.begin();curr!=characters.end();curr++)
      {
        if(*curr=='\\')
        {
          curr++;
        }
        else if(*curr=='\"' && !inSingleQuote && !inBoundComment && !inLineComment && !inDoubleQuote)
        {
          inDoubleQuote=true;
          //beginDoubleQuote=curr;
        }
        else if(*curr=='\"' && !inSingleQuote && !inBoundComment && !inLineComment &&  inDoubleQuote)
        {
          inDoubleQuote=false;
          //endDoubleQuote=std::next(curr,1);
          //curr=std::prev(characters.erase(beginDoubleQuote,endDoubleQuote),1);
        }
        else if(*curr=='\'' && !inDoubleQuote && !inBoundComment && !inLineComment && !inSingleQuote)
        {
          inSingleQuote=true;
          //beginSingleQuote=curr;
        }
        else if(*curr=='\'' && !inDoubleQuote && !inBoundComment && !inLineComment &&  inSingleQuote)
        {
          inSingleQuote=false;
          //endSingleQuote=std::next(curr,1);
          //curr=std::prev(characters.erase(beginSingleQuote,endSingleQuote),1);
        }
        else if(*curr=='/' && !inDoubleQuote && !inSingleQuote && !inBoundComment && !inLineComment && *std::next(curr,1)== '*')
        {
          inBoundComment=true;
          beginBoundComment=curr;
        }
        else if(*curr=='/' && !inDoubleQuote && !inSingleQuote && !inBoundComment && !inLineComment && *std::next(curr,1)== '/')
        {
          inLineComment=true;
          beginLineComment=curr;
        }
        else if(*curr=='*' && !inDoubleQuote && !inSingleQuote &&  inBoundComment && !inLineComment && *std::next(curr,1)== '/')
        {
          inBoundComment=false;
          endBoundComment=std::next(curr,2);
          curr=std::prev(characters.erase(beginBoundComment,endBoundComment),1);
        }
        else if((*curr=='\r' || *curr=='\n') && inLineComment && !inBoundComment)
        {
          inLineComment=false;
          inDoubleQuote=false;
          inSingleQuote=false;
          endLineComment=curr;
          curr=std::prev(characters.erase(beginLineComment,endLineComment),1);
        }
      }
      inDoubleQuote=false;
      inSingleQuote=false;
      bool inPreProcessor=false;
      for(curr=characters.begin();curr!=characters.end();curr++)
      {
        if(*curr=='\\')
        {
          if(inPreProcessor && *std::next(curr,1) == '\n' || *std::next(curr,1)== '\r')
          {
            curr=std::prev(characters.erase(curr, std::next(curr,1)),1);
          }
          else
          {
            curr++;
          }
        }
        else if(*curr=='\#')
        {
          inPreProcessor=true;
        }
        else if((*curr=='\n' || *curr=='\r') && inPreProcessor)
        {
          inPreProcessor=false;
        }
        else if(*curr=='\"' && !inSingleQuote && !inDoubleQuote)
        {
          inDoubleQuote=true;
          //beginDoubleQuote=curr;
        }
        else if(*curr=='\"' && !inSingleQuote &&  inDoubleQuote)
        {
          inDoubleQuote=false;
          //endDoubleQuote=std::next(curr,1);
          //curr=std::prev(characters.erase(beginDoubleQuote,endDoubleQuote),1);
        }
        else if(*curr=='\'' && !inDoubleQuote && !inSingleQuote)
        {
          inSingleQuote=true;
          //beginSingleQuote=curr;
        }
        else if(*curr=='\'' && !inDoubleQuote &&  inSingleQuote)
        {
          inSingleQuote=false;
          //endSingleQuote=std::next(curr,1);
          //curr=std::prev(characters.erase(beginSingleQuote,endSingleQuote),1);
        }
        else if(isspace(*curr) && (!isalnum(*std::prev(curr,1)) || !isalnum(*std::next(curr,1))) && !inSingleQuote && !inDoubleQuote )
        {
          curr=std::prev(characters.erase(curr),1);
        }
      }
      list<char>::iterator it;
      it=characters.begin();
      /**/
      /*
       *
       * / */
      //ssdfsdlfksdnf
      cout<<"This line shouldn't be touched /*sdfasdfasd*/"<<endl;
      cout<<"This line shouldn't be touched //asdfsdjfsdf"<<endl;
      string newFile = "C:\\Users\\Thomas\\Dropbox\\Programming\\C++\\QTFiles\\Projects\\IT287\\IT287-ResearchProject\\New\\"+fileNameList[i];
      pFile = fopen(newFile.c_str(), "w");//sdfsdfsf
      for(it=characters.begin(); it!=std::prev(characters.end(),1); ++it)
      {
        fputc(*it,/*sdfsdfs*/ pFile);
        printf("%c", *it);
      }
      fclose(pFile);
    }
    else
    {
      printf("Error opening file\n");
    }
  }
}

