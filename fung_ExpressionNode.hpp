#ifndef FUNG_ExpressionNode_HPP_INCLUDED
#define FUNG_ExpressionNode_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_identifier.hpp"
#include"fung_FunctionBody.hpp"
#include"fung_Expression.hpp"
#include<vector>


namespace fung{


class Context;
class ExpressionNode;

using ExpressionList = std::vector<Expression>;

using PairedExpression = std::pair<Expression,Expression>;


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
  sus,
  cal,
  acc,
  ina,
  der,

};


enum class
ExpressionNodeKind
{
  null,

  operator_,

  operation,

  paired,

  value,
  identifier,

  function_body,

  list,
 
};


union
ExpressionNodeData
{
  Mnemonic      mnemonic;
  Value            value;
  Identifier  identifier;

  FunctionBody const*  function_body;

  ExpressionList  list;

  ExpressionNodeData(){}
 ~ExpressionNodeData(){}

};


class
ExpressionNode
{
  ExpressionNodeKind  kind=ExpressionNodeKind::null;
  ExpressionNodeData  data;

  Expression   left;
  Expression  right;

  Value  operate(Context&  ctx, bool  multi) const;

  Value  choice(Context&  ctx, Value const&  lv, bool  multi) const;

  Value  tail_call(Context&  ctx) const;

  Value  log_or(Context&  ctx, Value const&  lv) const;
  Value  log_and(Context&  ctx, Value const&  lv) const;
  Value  cal(Context&  ctx, Value const&  lv) const;
  Value  sus(Context&  ctx, Value const&  lv) const;
  Value  binop(Context&  ctx, Value const&  lv, Value const&  rv) const;

public:
  ExpressionNode(){}
  explicit ExpressionNode(ExpressionNodeKind  k): kind(k){}
  explicit ExpressionNode(Identifier&&  id);
  explicit ExpressionNode(Value&&  v);
  explicit ExpressionNode(PairedExpression&&  p);
  explicit ExpressionNode(ExpressionList&&  ls);
  explicit ExpressionNode(FunctionBody const&  fnbd);
  ExpressionNode(Mnemonic  mn, Expression&&  l=Expression(),
                               Expression&&  r=Expression());
  ExpressionNode(ExpressionNode const&  rhs) noexcept{*this = rhs;}
  ExpressionNode(ExpressionNode&&       rhs) noexcept{*this = std::move(rhs);}
 ~ExpressionNode(){clear();}

  ExpressionNode&  operator=(ExpressionNode const&  rhs) noexcept;
  ExpressionNode&  operator=(ExpressionNode&&       rhs) noexcept;

  bool  operator==(ExpressionNodeKind  k) const{return kind == k;}
  bool  operator!=(ExpressionNodeKind  k) const{return kind != k;}

  operator bool() const{return kind != ExpressionNodeKind::null;}

  ExpressionNodeData const*  operator->() const{return &data;}

  void  clear();

  ExpressionNodeKind  get_kind() const{return kind;}

  Expression const&  get_left()  const{return  left;}
  Expression const&  get_right() const{return right;}

  bool   is_unary_operator() const;
  bool  is_binary_operator() const;
  bool          is_operand() const;

  Value  evaluate(Context&  ctx, bool  multi=false) const;

  void  print() const;

  static void  print_mnemonic(Mnemonic  mn);

};


constexpr bool
isexpr(char  c)
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')) ||
         ((c >= '0') && (c <= '9')) ||
         (c == '_') ||
         (c == '!') ||
         (c == '*') ||
         (c == '+') ||
         (c == '-') ||
         (c == ':') ||
         (c == '.') ||
         (c == '(') ||
         (c == '~') ||
         (c == '\"') ||
         (c == '\''));
}


List  to_list(ExpressionList const&  ls, Context&  ctx);

class GlobalSpace;

ExpressionList  read_expression_list(GlobalSpace&  sp, Cursor&  cur);


}




#endif




