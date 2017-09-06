#ifndef FUNG_Function_HPP_INCLUDED
#define FUNG_Function_HPP_INCLUDED


#include"fung_FunctionBody.hpp"
#include"fung_ExpressionNode.hpp"
#include"fung_parameter.hpp"
#include"fung_variable.hpp"
#include<string>
#include<initializer_list>


namespace fung{


class Context;


class
Function
{
  ParameterList  parameter_list;

  ValueKind  return_value_kind;

  FunctionBody const*  body;

public:
  Function(ParameterList&&  parals, ValueKind  retval_k, FunctionBody const*  body_=nullptr):
  parameter_list(std::move(parals)),
  return_value_kind(retval_k),
  body(body_){}


  ValueKind  get_return_value_kind() const{return return_value_kind;}

  ParameterList const&  get_parameter_list() const{return parameter_list;}

  FunctionBody const*  get_body() const{return body;}

  void  print() const;

};




}




#endif




