#ifndef FUNG_Function_HPP_INCLUDED
#define FUNG_Function_HPP_INCLUDED


#include"fung_statement.hpp"
#include"fung_expression.hpp"
#include"fung_parameter.hpp"
#include"fung_space.hpp"
#include"fung_context.hpp"
#include<string>
#include<initializer_list>


namespace fung{


using FunctionBody = std::vector<Statement>;



class
Function: public Space
{
  std::string  name;

  ParameterList  parameter_list;

  ValueKind  return_value_kind;

  FunctionBody  body;

public:
  Function(Space&  parent_sp, std::string&&  name_, ParameterList&&  parals, ValueKind  retval_k, FunctionBody&&  body_):
  Space(parent_sp),
  name(std::move(name_)),
  parameter_list(std::move(parals)),
  return_value_kind(retval_k),
  body(std::move(body_)){}


  std::string const&  get_name() const{return name;}

  Value  operator()(Context&  ctx, std::initializer_list<Value>  args={}) const;
  Value  operator()(Context&  ctx, ValueList const&  args) const;

  ParameterList const&  get_parameter_list() const{return parameter_list;}

  void  print() const;

};




}




#endif




