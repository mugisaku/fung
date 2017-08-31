#ifndef FUNG_Space_HPP_INCLUDED
#define FUNG_Space_HPP_INCLUDED


#include<vector>
#include<list>
#include<memory>
#include<string>


namespace fung{


class Constant;
class Expression;
class Value;
class GlobalSpace;


class
Space
{
  GlobalSpace*  global=nullptr;

  std::vector<std::unique_ptr<Constant>>  constant_list;

  Space*  parent=nullptr;

public:
  Space(Space&  parent_);

  void  append_constant(Constant*  con){constant_list.emplace_back(con);}

  std::unique_ptr<Constant> const&  find_constant(std::string const&  name) const;

  void  print() const;

};




}




#endif




