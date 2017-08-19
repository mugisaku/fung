#ifndef FUNG_Function_HPP_INCLUDED
#define FUNG_Function_HPP_INCLUDED


#include"fung_variable.hpp"
#include"fung_statement.hpp"


namespace fung{




using FunctionBody = std::vector<Statement>;



class
Function
{
  ParameterList  parameter_list;

  std::string  return_type;

  FunctionBody  body;

public:
  Function(ParameterList&&  parals, std::string&&  return_type_, FunctionBody&&  body_):
  parameter_list(std::move(parals)),
  return_type(std::move(return_type_)),
  body(std::move(body_)){}

  ParameterList const&  get_parameter_list() const{return parameter_list;}

  void  print() const;

};




}




#endif




