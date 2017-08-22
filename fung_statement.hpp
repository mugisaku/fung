#ifndef FUNG_Statement_HPP_INCLUDED
#define FUNG_Statement_HPP_INCLUDED


#include<string>
#include"fung_expression.hpp"


namespace fung{




class Context;


enum class
StatementKind
{
  null,

  let,
  return_,

};


struct
LetStatement
{
  std::string  identifier;

  Expression  expression;

  LetStatement(std::string&&  id, Expression&&  expr): identifier(std::move(id)), expression(std::move(expr)){}

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
  Statement(LetStatement&&  bin);
  Statement(ReturnStatement&&  ret);
  Statement(Statement const&  rhs) noexcept{*this = rhs;}
  Statement(Statement&&       rhs) noexcept{*this = std::move(rhs);}
 ~Statement(){clear();}

  Statement&  operator=(Statement const&  rhs) noexcept;
  Statement&  operator=(Statement&&       rhs) noexcept;

  Expression const*  operator->() const{return &expression;}

  bool  operator==(StatementKind  k) const{return kind == k;}


  void  clear();

  bool  execute(Context&  ctx) const;

  void  print() const;

};


}




#endif




