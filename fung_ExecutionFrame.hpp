#ifndef FUNG_ExecutionFrame_HPP_INCLUDED
#define FUNG_ExecutionFrame_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_error.hpp"
#include"fung_function.hpp"
#include<vector>
#include<memory>


namespace fung{


struct
Variable
{
  std::string  name;

  Value  value;

  Variable(std::string const&  name_, Value const&  v): name(name_), value(v){}

};


class
ExecutionFrame
{
  Function const&  function;

  std::vector<Variable>  variable_list;

  FunctionBody::const_iterator  current;
  FunctionBody::const_iterator      end;

public:
  ExecutionFrame(Function const&  fn);

  Function const&  get_function() const{return function;}

  void  append_variable(Variable&&  var);

  Variable const*  find_variable(std::string const&  name) const;

  Statement const*  get_next_statement();

};




}




#endif




