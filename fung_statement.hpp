#ifndef FUNG_Statement_HPP_INCLUDED
#define FUNG_Statement_HPP_INCLUDED


#include<string>
#include<vector>
#include"fung_Expression.hpp"


namespace fung{




class Context;


enum class
StatementKind
{
  null,

  interrupt,
  print,
  let,
  return_,

};


struct
LetStatement
{
  std::string  identifier;

  Expression  expression;

  LetStatement(std::string&&  id, Expression&&  expr):
  identifier(std::move(id)),
  expression(std::move(expr)){}

};


struct
PrintStatement
{
  Expression  expression;

  PrintStatement(Expression&&  expr):
  expression(std::move(expr)){}

};


struct
ReturnStatement
{
  Expression  expression;

  ReturnStatement(Expression&&  expr):
  expression(std::move(expr)){}

};


struct InterruptStatement{};


class
Statement
{
  StatementKind  kind=StatementKind::null;

  std::string  identifier;

  Expression  expression;

public:
  Statement(){}
  Statement(InterruptStatement  intstmt);
  Statement(LetStatement&&  bin);
  Statement(ReturnStatement&&  ret);
  Statement(PrintStatement&&  prn);
  Statement(Statement const&  rhs) noexcept{*this = rhs;}
  Statement(Statement&&       rhs) noexcept{*this = std::move(rhs);}
 ~Statement(){clear();}

  Statement&  operator=(Statement const&  rhs) noexcept;
  Statement&  operator=(Statement&&       rhs) noexcept;

  ExpressionNode const*  operator->() const{return &*expression;}

  operator bool() const{return kind != StatementKind::null;}

  bool  operator==(StatementKind  k) const{return kind == k;}


  void  clear();

  StatementKind  get_kind() const{return kind;}

  std::string const&  get_identifier() const{return identifier;}
  Expression const&   get_expression() const{return expression;}


  void  print() const;

};


using StatementList = std::vector<Statement>;

class Cursor;

StatementList  read_statement_list(Cursor&  cur, std::string const&  fn_name);

}




#endif




