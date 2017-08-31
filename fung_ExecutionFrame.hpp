#ifndef FUNG_ExecutionFrame_HPP_INCLUDED
#define FUNG_ExecutionFrame_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_error.hpp"
#include"fung_function.hpp"
#include"fung_variable.hpp"
#include<vector>


namespace fung{


class
ExecutionFrame
{
  std::string const&  function_name;

  Function const&  function;

  std::vector<Variable>  variable_list;

  FunctionBody::const_iterator  current;
  FunctionBody::const_iterator      end;

public:
  ExecutionFrame(std::string const&  fn_name, Function const&  fn);

  std::string const&  get_function_name() const{return function_name;}
  Function const&  get_function() const{return function;}

  void  append_variable(Variable&&  var);

  Variable const*  find_variable(std::string const&  name) const;

  Statement const*  get_next_statement();

};




}




#endif




