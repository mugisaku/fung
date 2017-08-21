#ifndef FUNG_Space_HPP_INCLUDED
#define FUNG_Space_HPP_INCLUDED


#include<vector>
#include<list>
#include<memory>


namespace fung{


class Variable;
class GlobalSpace;


class
Space
{
  GlobalSpace*  global=nullptr;

  Space*  parent=nullptr;

public:
  Space(Space&  parent_);

  void  print() const;

};




}




#endif




