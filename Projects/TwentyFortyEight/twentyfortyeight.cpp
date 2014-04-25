#include "twentyfortyeight.h"
#include<stdlib.h>
#include <time.h>
using namespace std;

TwentyFortyEight::TwentyFortyEight()
{
  this->cost=0;
  this->grid[0]=0;
  this->grid[1]=0;
  this->grid[2]=0;
  this->grid[3]=0;
  this->grid[4]=0;
  this->grid[5]=0;
  this->grid[6]=0;
  this->grid[7]=0;
  this->grid[8]=0;
  this->grid[9]=0;
  this->grid[10]=0;
  this->grid[11]=0;
  this->grid[12]=0;
  this->grid[13]=0;
  this->grid[14]=0;
  this->grid[15]=0;
  this->addRandom();
}
TwentyFortyEight::TwentyFortyEight(ProblemState*orig)
{
  TwentyFortyEight*original=(TwentyFortyEight*)orig;
  this->setCost(original->getCost()+1);
  this->grid[0]=original->grid[0];
  this->grid[1]=original->grid[1];
  this->grid[2]=original->grid[2];
  this->grid[3]=original->grid[3];
  this->grid[4]=original->grid[4];
  this->grid[5]=original->grid[5];
  this->grid[6]=original->grid[6];
  this->grid[7]=original->grid[7];
  this->grid[8]=original->grid[8];
  this->grid[9]=original->grid[9];
  this->grid[10]=original->grid[10];
  this->grid[11]=original->grid[11];
  this->grid[12]=original->grid[12];
  this->grid[13]=original->grid[13];
  this->grid[14]=original->grid[14];
  this->grid[15]=original->grid[15];
  this->direction=-1;
}
TwentyFortyEight::~TwentyFortyEight()
{
  //Nothing to do
}
TwentyFortyEight TwentyFortyEight::operator=(const TwentyFortyEight& that)
{
  if (this == &that)
  {
    return*this;
  }
  this->grid[0]=that.grid[0];
  this->grid[1]=that.grid[1];
  this->grid[2]=that.grid[2];
  this->grid[3]=that.grid[3];
  this->grid[4]=that.grid[4];
  this->grid[5]=that.grid[5];
  this->grid[6]=that.grid[6];
  this->grid[7]=that.grid[7];
  this->grid[8]=that.grid[8];
  this->grid[9]=that.grid[9];
  this->grid[10]=that.grid[10];
  this->grid[11]=that.grid[11];
  this->grid[12]=that.grid[12];
  this->grid[13]=that.grid[13];
  this->grid[14]=that.grid[14];
  this->grid[15]=that.grid[15];
  this->cost=that.cost;
  this->direction=that.direction;
  return*this;
}
bool TwentyFortyEight::operator==(const TwentyFortyEight&that) const
{
  if(this==&that)
  {
    return true;
  }
  if(this->grid[0]!=that.grid[0])return false;
  if(this->grid[1]!=that.grid[1])return false;
  if(this->grid[2]!=that.grid[2])return false;
  if(this->grid[3]!=that.grid[3])return false;
  if(this->grid[4]!=that.grid[4])return false;
  if(this->grid[5]!=that.grid[5])return false;
  if(this->grid[6]!=that.grid[6])return false;
  if(this->grid[7]!=that.grid[7])return false;
  if(this->grid[8]!=that.grid[8])return false;
  if(this->grid[9]!=that.grid[9])return false;
  if(this->grid[10]!=that.grid[10])return false;
  if(this->grid[11]!=that.grid[11])return false;
  if(this->grid[12]!=that.grid[12])return false;
  if(this->grid[13]!=that.grid[13])return false;
  if(this->grid[14]!=that.grid[14])return false;
  if(this->grid[15]!=that.grid[15])return false;
  return true;
}
bool TwentyFortyEight::operator !=(const TwentyFortyEight&that) const
{
  return !(*this==that);
}
vector<shared_ptr<ProblemState> > TwentyFortyEight::createSuccessorStates()
{
  vector<shared_ptr<ProblemState> > states;
  states.reserve(4);
  TwentyFortyEight left = new TwentyFortyEight(this);
  unsigned short int* array = new unsigned short int[4];
  array[0]=this->grid[0];
  array[1]=this->grid[1];
  array[2]=this->grid[2];
  array[3]=this->grid[3];
  array=packAndCombine(array);
  left.grid[0]=array[0];
  left.grid[1]=array[1];
  left.grid[2]=array[2];
  left.grid[3]=array[3];
  array[0]=this->grid[4];
  array[1]=this->grid[5];
  array[2]=this->grid[6];
  array[3]=this->grid[7];
  array=packAndCombine(array);
  left.grid[4]=array[0];
  left.grid[5]=array[1];
  left.grid[6]=array[2];
  left.grid[7]=array[3];
  array[0]=this->grid[8];
  array[1]=this->grid[9];
  array[2]=this->grid[10];
  array[3]=this->grid[11];
  array=packAndCombine(array);
  left.grid[8]=array[0];
  left.grid[9]=array[1];
  left.grid[10]=array[2];
  left.grid[11]=array[3];
  array[0]=this->grid[12];
  array[1]=this->grid[13];
  array[2]=this->grid[14];
  array[3]=this->grid[15];
  array=packAndCombine(array);
  left.grid[12]=array[0];
  left.grid[13]=array[1];
  left.grid[14]=array[2];
  left.grid[15]=array[3];
  left.direction=0;
  if(*this!=left)
  {
    left.addRandom();
    states.push_back(shared_ptr<TwentyFortyEight>(&left));
  }

  TwentyFortyEight right = new TwentyFortyEight(this);
  array[0]=this->grid[3];
  array[1]=this->grid[2];
  array[2]=this->grid[1];
  array[3]=this->grid[0];
  array=packAndCombine(array);
  right.grid[3]=array[0];
  right.grid[2]=array[1];
  right.grid[1]=array[2];
  right.grid[0]=array[3];
  array[0]=this->grid[7];
  array[1]=this->grid[6];
  array[2]=this->grid[5];
  array[3]=this->grid[4];
  array=packAndCombine(array);
  right.grid[7]=array[0];
  right.grid[6]=array[1];
  right.grid[5]=array[2];
  right.grid[4]=array[3];
  array[0]=this->grid[11];
  array[1]=this->grid[10];
  array[2]=this->grid[9];
  array[3]=this->grid[8];
  array=packAndCombine(array);
  right.grid[11]=array[0];
  right.grid[10]=array[1];
  right.grid[9]=array[2];
  right.grid[8]=array[3];
  array[0]=this->grid[15];
  array[1]=this->grid[14];
  array[2]=this->grid[13];
  array[3]=this->grid[12];
  array=packAndCombine(array);
  right.grid[15]=array[0];
  right.grid[14]=array[1];
  right.grid[13]=array[2];
  right.grid[12]=array[3];
  right.direction=1;
  if(*this!=right)
  {
    right.addRandom();
    states.push_back(shared_ptr<TwentyFortyEight>(&right));
  }

  TwentyFortyEight up = new TwentyFortyEight(this);

  array[0]=this->grid[0];
  array[1]=this->grid[4];
  array[2]=this->grid[8];
  array[3]=this->grid[12];
  array=packAndCombine(array);
  up.grid[0]=array[0];
  up.grid[4]=array[1];
  up.grid[8]=array[2];
  up.grid[12]=array[3];
  array[0]=this->grid[1];
  array[1]=this->grid[5];
  array[2]=this->grid[9];
  array[3]=this->grid[13];
  array=packAndCombine(array);
  up.grid[1]=array[0];
  up.grid[5]=array[1];
  up.grid[9]=array[2];
  up.grid[13]=array[3];
  array[0]=this->grid[2];
  array[1]=this->grid[6];
  array[2]=this->grid[10];
  array[3]=this->grid[14];
  array=packAndCombine(array);
  up.grid[2]=array[0];
  up.grid[6]=array[1];
  up.grid[10]=array[2];
  up.grid[14]=array[3];
  array[0]=this->grid[3];
  array[1]=this->grid[7];
  array[2]=this->grid[11];
  array[3]=this->grid[15];
  array=packAndCombine(array);
  up.grid[3]=array[0];
  up.grid[7]=array[1];
  up.grid[11]=array[2];
  up.grid[15]=array[3];
  up.direction=2;
  if(*this!=up)
  {
    up.addRandom();
    states.push_back(shared_ptr<TwentyFortyEight>(&up));
  }

  TwentyFortyEight down = new TwentyFortyEight(this);
  array[0]=this->grid[12];
  array[1]=this->grid[8];
  array[2]=this->grid[4];
  array[3]=this->grid[0];
  array=packAndCombine(array);
  down.grid[12]=array[0];
  down.grid[8]=array[1];
  down.grid[4]=array[2];
  down.grid[0]=array[3];
  array[0]=this->grid[13];
  array[1]=this->grid[9];
  array[2]=this->grid[5];
  array[3]=this->grid[1];
  array=packAndCombine(array);
  down.grid[13]=array[0];
  down.grid[9]=array[1];
  down.grid[4]=array[2];
  down.grid[1]=array[3];
  array[0]=this->grid[14];
  array[1]=this->grid[10];
  array[2]=this->grid[6];
  array[3]=this->grid[2];
  array=packAndCombine(array);
  down.grid[14]=array[0];
  down.grid[10]=array[1];
  down.grid[6]=array[2];
  down.grid[2]=array[3];
  array[0]=this->grid[15];
  array[1]=this->grid[11];
  array[2]=this->grid[7];
  array[3]=this->grid[3];
  array=packAndCombine(array);
  down.grid[15]=array[0];
  down.grid[11]=array[1];
  down.grid[7]=array[2];
  down.grid[3]=array[3];
  down.direction=3;
  if(*this!=down)
  {
    down.addRandom();
    states.push_back(shared_ptr<TwentyFortyEight>(&down));
  }
  delete [] array;
  return states;
}

bool TwentyFortyEight::goalTest()
{
  if(grid[0]==goal)return true;
  if(grid[1]==goal)return true;
  if(grid[2]==goal)return true;
  if(grid[3]==goal)return true;
  if(grid[4]==goal)return true;
  if(grid[5]==goal)return true;
  if(grid[6]==goal)return true;
  if(grid[7]==goal)return true;
  if(grid[8]==goal)return true;
  if(grid[9]==goal)return true;
  if(grid[10]==goal)return true;
  if(grid[11]==goal)return true;
  if(grid[12]==goal)return true;
  if(grid[13]==goal)return true;
  if(grid[14]==goal)return true;
  if(grid[15]==goal)return true;
  return false;
}

void TwentyFortyEight::printState()
{
  printf("+------+------+------+------+ $s\n", this->direction);
  printf("|      |      |      |      |\n|");
  if(!this->grid[0])printf("      |");
  else printf(" %4d |",this->grid[0]);
  if(!this->grid[1])printf("      |");
  else printf(" %4d |",this->grid[1]);
  if(!this->grid[2])printf("      |");
  else printf(" %4d |",this->grid[2]);
  if(!this->grid[3])printf("      |");
  else printf(" %4d |",this->grid[3]);
  printf("\n|      |      |      |      |\n+------+------+------+------+\n");
  printf("|      |      |      |      |\n|");
  if(!this->grid[4])printf("      |");
  else printf(" %4d |",this->grid[4]);
  if(!this->grid[5])printf("      |");
  else printf(" %4d |",this->grid[5]);
  if(!this->grid[6])printf("      |");
  else printf(" %4d |",this->grid[6]);
  if(!this->grid[7])printf("      |");
  else printf(" %4d |",this->grid[7]);
  printf("\n|      |      |      |      |\n+------+------+------+------+\n");
  printf("|      |      |      |      |\n|");
  if(!this->grid[8])printf("      |");
  else printf(" %4d |",this->grid[8]);
  if(!this->grid[9])printf("      |");
  else printf(" %4d |",this->grid[9]);
  if(!this->grid[10])printf("      |");
  else printf(" %4d |",this->grid[10]);
  if(!this->grid[11])printf("      |");
  else printf(" %4d |",this->grid[11]);
  printf("\n|      |      |      |      |\n+------+------+------+------+\n");
  printf("|      |      |      |      |\n|");
  if(!this->grid[12])printf("      |");
  else printf(" %4d |",this->grid[12]);
  if(!this->grid[13])printf("      |");
  else printf(" %4d |",this->grid[13]);
  if(!this->grid[14])printf("      |");
  else printf(" %4d |",this->grid[14]);
  if(!this->grid[15])printf("      |");
  else printf(" %4d |",this->grid[15]);
  printf("\n|      |      |      |      |\n+------+------+------+------+\n");
  printf("Moves made = %d\n\n", this->cost);
}

void TwentyFortyEight::addRandom()
{
  srand((unsigned int)time(NULL));
  char nextNum;
  if(rand()%1+1)
  {
    nextNum=2;
  }
  else
  {
    nextNum=4;
  }
  vector<char> potentialPlaces;
  if(!this->grid[0])potentialPlaces.push_back(0);
  if(!this->grid[1])potentialPlaces.push_back(1);
  if(!this->grid[2])potentialPlaces.push_back(2);
  if(!this->grid[3])potentialPlaces.push_back(3);
  if(!this->grid[4])potentialPlaces.push_back(4);
  if(!this->grid[5])potentialPlaces.push_back(5);
  if(!this->grid[6])potentialPlaces.push_back(6);
  if(!this->grid[7])potentialPlaces.push_back(7);
  if(!this->grid[8])potentialPlaces.push_back(8);
  if(!this->grid[9])potentialPlaces.push_back(9);
  if(!this->grid[10])potentialPlaces.push_back(10);
  if(!this->grid[11])potentialPlaces.push_back(11);
  if(!this->grid[12])potentialPlaces.push_back(12);
  if(!this->grid[13])potentialPlaces.push_back(13);
  if(!this->grid[14])potentialPlaces.push_back(14);
  if(!this->grid[15])potentialPlaces.push_back(15);
  if(!potentialPlaces.size())return;
  char randomNum=rand()%potentialPlaces.size();
  this->grid[potentialPlaces[randomNum]]=nextNum;
}
unsigned short int* TwentyFortyEight::packAndCombine(unsigned short int *array)
{
  char i=0,j;
  while(i<4)
  {
    if(!array[i])
    {
      j=i;
      while(i<4)
      {
        if(!array[i])
        {
          i++;
        }
        else
        {
          array[j]=array[i];
          array[i]=0;
          break;
        }
      }
      i=j;
    }
    if(i!=3&&array[i]&&array[i]==array[i+1])
    {
      array[i]*=2;
      array[i+1]=0;
    }
    i++;
  }
  return array;
}


