#ifndef FUNG_Variable_HPP_INCLUDED
#define FUNG_Variable_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_expression.hpp"
#include<string>
#include<cstdio>
#include<memory>


namespace fung{


class GlobalSpace;


struct
Variable
{
  std::string  name;

  Expression  expression;

  Value  value;

  Variable(std::string const&  name_, Value const&  v): name(name_), value(v){}

};


struct
Constant
{
  ValueKind  value_kind;

  std::string  name;

  ExpressionList  expression_list;

  Value  value;

  Constant(ValueKind  k, std::string&&  name_, ExpressionList&&  ls);
  Constant(std::string&&  name_, Value&&  v);

  Value const&  get_value(std::unique_ptr<GlobalSpace> const&  gsp);

  void  print() const;

};


}




#endif




