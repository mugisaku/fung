#ifndef FUNG_Function_HPP_INCLUDED
#define FUNG_Function_HPP_INCLUDED


#include"fung_statement.hpp"
#include"fung_expression.hpp"
#include"fung_parameter.hpp"
#include"fung_space.hpp"
#include"fung_variable.hpp"
#include<string>
#include<initializer_list>


namespace fung{


class Context;


using FunctionBody = std::vector<Statement>;



class
Function: public Space
{
  ParameterList  parameter_list;

  ValueKind  return_value_kind;

  FunctionBody  body;

public:
  Function(Space&  parent_sp, ParameterList&&  parals, ValueKind  retval_k, FunctionBody&&  body_):
  Space(parent_sp),
  parameter_list(std::move(parals)),
  return_value_kind(retval_k),
  body(std::move(body_)){}


  Value  operator()(std::string const&  name, Context&  ctx, std::initializer_list<Value>  args={}) const;
  Value  operator()(std::string const&  name, Context&  ctx, List const&  args) const;

  ParameterList const&  get_parameter_list() const{return parameter_list;}

  FunctionBody const&  get_body() const{return body;}

  void  print() const;

};




}




#endif




