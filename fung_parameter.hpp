#ifndef FUNG_Parameter_HPP_INCLUDED
#define FUNG_Parameter_HPP_INCLUDED


#include"fung_ValueKind.hpp"
#include"fung_expression.hpp"
#include<vector>


namespace fung{


class
Parameter
{
  ValueKind  value_kind;

  std::string  name;

  Expression  expression;

public:
  Parameter(ValueKind  k, std::string&&  name_, Expression&&  expr=Expression()):
  value_kind(k),
  name(std::move(name_)),
  expression(std::move(expr)){}

  bool  operator==(std::string const&  name_) const{return name == name_;}

  Expression const*  operator->() const{return &expression;}

  ValueKind  get_value_kind() const{return value_kind;}

  std::string const&  get_name() const{return name;}

  Expression const&  get_expression() const{return expression;}

  void  print() const;

};


using ParameterList = std::vector<Parameter>;


class Cursor;

ParameterList  read_parameter_list(Cursor&  cur);


}




#endif




