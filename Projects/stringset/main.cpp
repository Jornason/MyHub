#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
  vector<string> s,s2,s3,s4;
  s.push_back("a");
  s.push_back("b");
  s.push_back("c");
  s.push_back("d");
  s.push_back("e");

  s2=s;
  for(int i=0;i<15;i++)
  {
    s3=s2;
    for(int j=0;j<5;j++)
    {
      for(int k=0;k<s3.size();k++)
      {
        s3[k]=s3[k]+s[k]
      }
    }
  }
}

