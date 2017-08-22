#ifndef FUNG_Expression_HPP_INCLUDED
#define FUNG_Expression_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_identifier.hpp"
#include<vector>


namespace fung{


class Context;
class Expression;

using ExpressionList = std::vector<Expression>;


enum class
Mnemonic
{
  nop,
  add,
  sub,
  mul,
  div,
  rem,
  shl,
  shr,
  bit_and,
  bit_or,
  bit_xor,
  bit_not,
  eq,
  neq,
  lt,
  lteq,
  gt,
  gteq,
  log_or,
  log_and,
  log_not,
  neg,
  cho,
  eth,
  sus,
  cal,

};


enum class
ExpressionKind
{
  null,

  operator_,

  operation,

  value,
  identifier,

  list,
 
};


union
ExpressionData
{
  Mnemonic      mnemonic;
  Value            value;
  Identifier  identifier;

  std::vector<Expression>  list;

  ExpressionData(){}
 ~ExpressionData(){}

};


class
Expression
{
  ExpressionKind  kind=ExpressionKind::null;
  ExpressionData  data;

  Expression*   left=nullptr;
  Expression*  right=nullptr;

  Value  operate(Context&  ctx, bool  b) const;

public:
  Expression(){}
  explicit Expression(ExpressionKind  k): kind(k){}
  explicit Expression(Identifier&&  id);
  explicit Expression(Value&&  v);
  explicit Expression(ExpressionList&&  ls);
  Expression(Mnemonic  mn, Expression*  l=nullptr, Expression*  r=nullptr);
  Expression(Expression const&  rhs) noexcept{*this = rhs;}
  Expression(Expression&&       rhs) noexcept{*this = std::move(rhs);}
 ~Expression(){clear();}

  Expression&  operator=(Expression const&  rhs) noexcept;
  Expression&  operator=(Expression&&       rhs) noexcept;

  bool  operator==(ExpressionKind  k) const{return kind == k;}
  bool  operator!=(ExpressionKind  k) const{return kind != k;}

  operator bool() const{return kind != ExpressionKind::null;}

  ExpressionData const*  operator->() const{return &data;}

  void  clear();

  bool   is_unary_operator() const;
  bool  is_binary_operator() const;
  bool          is_operand() const;

  Value  evaluate(Context&  ctx, bool  b=true) const;

  void  print() const;

  static void  print_mnemonic(Mnemonic  mn);

};


ValueList  to_value_list(ExpressionList const&  ls, Context&  ctx);

using ArgumentList = std::vector<Expression>;

ArgumentList  read_argment_list(Cursor&  cur);


}




#endif




