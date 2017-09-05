#ifndef FUNG_ExecutionFrame_HPP_INCLUDED
#define FUNG_ExecutionFrame_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_error.hpp"
#include"fung_function.hpp"
#include"fung_variable.hpp"
#include<vector>


namespace fung{


enum class
ExecutionResult
{
  no_value_was_returned,
  value_was_returned,
  interrupted,
  error,

};


class
ExecutionFrame
{
  std::string const&  function_name;

  Function const*  function=nullptr;

  VariableList  argument_list;
  VariableList  variable_list;

  FunctionBody::const_iterator    begin;
  FunctionBody::const_iterator  current;
  FunctionBody::const_iterator      end;

public:
  ExecutionFrame(std::string const&  fn_name, Function const&  fn, List&&  valls);
  ExecutionFrame(StatementList const&  stmtls);

  std::string const&  get_function_name() const{return function_name;}

  Function const*  get_function() const{return function;}

  void  update_argument_list(List&&  valls);

  void  append_variable(Variable&&  var);

  Variable const*  find_variable(std::string const&  name) const;

  ExecutionResult  step(Context&  ctx, Value&  retval_buf);

};




}




#endif




