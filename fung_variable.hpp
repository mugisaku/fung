#ifndef FUNG_Variable_HPP_INCLUDED
#define FUNG_Variable_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_ExpressionNode.hpp"
#include<string>
#include<cstdio>
#include<vector>
#include<memory>


namespace fung{


class GlobalSpace;


struct
Variable
{
  ValueKind  value_kind;

  std::string  name;

  ExpressionList  expression_list;

  Value  value;

  Variable(ValueKind  k, std::string&&  name_, ExpressionList&&  ls);
  Variable(std::string&&  name_, Value&&  v);

  Value const&  get_value(std::unique_ptr<GlobalSpace> const&  gsp);

  void  print() const;

};


}




#endif




