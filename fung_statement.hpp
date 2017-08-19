#ifndef FUNG_Statement_HPP_INCLUDED
#define FUNG_Statement_HPP_INCLUDED


#include<string>
#include"fung_expression.hpp"


namespace fung{




enum class
StatementKind
{
  null,
  bind,
  return_,

};


struct
ReturnStatement
{
  Expression  expression;

  ReturnStatement(Expression&&  expr): expression(std::move(expr)){}

};


class
Statement
{
  StatementKind  kind=StatementKind::null;

  std::string  identifier;
  Expression   expression;

public:
  Statement(){}
  Statement(ReturnStatement&&  ret);
  Statement(Statement const&  rhs) noexcept{*this = rhs;}
  Statement(Statement&&       rhs) noexcept{*this = std::move(rhs);}
 ~Statement(){clear();}

  Statement&  operator=(Statement const&  rhs) noexcept;
  Statement&  operator=(Statement&&       rhs) noexcept;

  void  clear();
  void  print() const;

};


}




#endif




