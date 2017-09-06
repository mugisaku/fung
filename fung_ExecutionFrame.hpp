#ifndef FUNG_ExecutionFrame_HPP_INCLUDED
#define FUNG_ExecutionFrame_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_error.hpp"
#include"fung_FunctionBody.hpp"
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

  FunctionBody const&  function_body;

  ValueKind  return_value_kind=ValueKind::null;

  std::vector<Variable>  argument_list;
  std::vector<Variable>  variable_list;

  StatementList::const_iterator  current;

  void  update_argument_list(List&&  valls);

  ExecutionFrame const*  parent=nullptr;

public:
  ExecutionFrame(std::string const&  fn_name, Function const&  fn, List&&  valls);
  ExecutionFrame(FunctionBody const&  body, ExecutionFrame const*  parent_=nullptr);

  std::string const&  get_function_name() const{return function_name;}

  FunctionBody const&  get_function_body() const{return function_body;}

  void  append_variable(Variable&&  var);

  Variable const*  find_variable(std::string const&  name) const;

  ExecutionResult  step(Context&  ctx, Value&  retval_buf);

};




}




#endif




