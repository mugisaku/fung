#ifndef FUNG_Variable_HPP_INCLUDED
#define FUNG_Variable_HPP_INCLUDED


#include"fung_ValueKind.hpp"
#include"fung_expression.hpp"
#include<memory>
#include<vector>


namespace fung{


class Function;
class Space;


class
Variable
{
  ValueKind  value_kind;

  std::string  name;

  Expression  expression;

  std::unique_ptr<Function>  function;

public:
  Variable(ValueKind  k, std::string&&  name_, Expression&&  expr=Expression()):
  value_kind(k),
  name(std::move(name_)),
  expression(std::move(expr)){}

  Variable(std::string&&  name_, Function*  fn):
  value_kind(ValueKind::function),
  name(std::move(name_)),
  function(fn){}

  bool  operator==(std::string const&  name_) const{return name == name_;}

  Expression const*  operator->() const{return &expression;}

  ValueKind  get_value_kind() const{return value_kind;}

  std::string const&  get_name() const{return name;}

  Expression const&  get_expression() const{return expression;}

  std::unique_ptr<Function> const&  get_function() const{return function;}

  Value  get_value(Space const&  sp) const;

  void  print() const;

};


using ParameterList = std::vector<Variable>;


class Cursor;

ParameterList  read_parameter_list(Cursor&  cur);


}




#endif




