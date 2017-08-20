#ifndef FUNG_Space_HPP_INCLUDED
#define FUNG_Space_HPP_INCLUDED


#include<vector>
#include<list>
#include<memory>


namespace fung{


class Variable;


class
Space
{
  Space*  parent=nullptr;

  std::list<Variable>  variable_list;

  std::vector<std::unique_ptr<Space>>  children;

public:
  Space(Space*  parent_=nullptr): parent(parent_){}

  void  append_variable(Variable&&  v);

  Variable const*  find_variable(std::string const&  name) const;

  void  print() const;

};




}




#endif




