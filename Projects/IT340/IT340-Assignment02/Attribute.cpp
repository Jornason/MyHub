#include"Attribute.h"
Attribute::Attribute()
{
  this->name="DEFAULT";
}
Attribute::Attribute(Attribute* orig)
{
  this->name=orig->name;
  this->valLst=orig->valLst;
}
Attribute Attribute::operator=(const Attribute&rhs)
{
  if(this==&rhs)
  {
    return*this;
  }
  this->name=rhs.name;
  this->valLst=rhs.valLst;
  return*this;
}
Attribute::~Attribute()
{
  //Nothing to do
}
void Attribute::setName(string&name)
{
  this->name=name;
}
void Attribute::addVal(string&val)
{
  this->valLst.push_back(make_shared<string>(val));
}
string&Attribute::getName()
{
  return this->name;
}
string&Attribute::getVal(unsigned int index)
{
  return*this->valLst[index];
}
unsigned int Attribute::getNumVals()
{
  return this->valLst.size();
}
