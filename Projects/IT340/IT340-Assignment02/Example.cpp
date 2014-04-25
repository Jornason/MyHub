#include"Example.h"
Example::Example()
{
  //No classification has been set yet
  this->classIndx=-1;
}
Example::Example(Example*orig)
{
  this->attrValIndxs=orig->attrValIndxs;
  this->classIndx=orig->classIndx;
}
Example Example::operator=(const Example&rhs)
{
  if(this==&rhs) return*this;
  this->attrValIndxs=rhs.attrValIndxs;
  this->classIndx=rhs.classIndx;
  return*this;
}
Example::~Example()
{
  //Nothing to do
}
void Example::addAttrValIndx(int attrValIndx)
{
  this->attrValIndxs.push_back(attrValIndx);
}
void Example::setClass(int classIndx)
{
  this->classIndx=classIndx;
}
int Example::getAttrValIndx(int index)
{
  return this->attrValIndxs[index];
}
int Example::getClass()
{
  return this->classIndx;
}
