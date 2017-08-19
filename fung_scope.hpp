#ifndef FUNG_Scope_HPP_INCLUDED
#define FUNG_Scope_HPP_INCLUDED


#include<vector>
#include<list>
#include<memory>


namespace fung{


class Variable;


class
Scope
{
  Scope*  parent=nullptr;

  std::list<Variable>  variable_list;

  std::vector<std::unique_ptr<Scope>>  children;

public:
  Scope(Scope*  parent_=nullptr): parent(parent_){}

  void  append_variable(Variable&&  v);

  Variable const*  find_variable(std::string const&  name) const;

  void  print() const;

};




}




#endif




