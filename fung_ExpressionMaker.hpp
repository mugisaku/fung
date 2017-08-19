#ifndef FUNG_ExpressionMaker_HPP_INCLUDED
#define FUNG_ExpressionMaker_HPP_INCLUDED


#include"fung_expression.hpp"
#include"fung_cursor.hpp"
#include"fung_TinyString.hpp"


namespace fung{


class TinyString;


class
ExpressionMaker
{
  std::vector<char>       unop_stack;
  std::vector<Mnemonic>  binop_stack;

  std::vector<Expression>  output;

  bool  last_is_operand = false;

  TinyString  last_operator;

  bool  process_operator(Cursor&  cur, TinyString const&  o);

  void  push_operand(Expression&&  expr);

public:
  Expression  operator()(Cursor&  cur);

  TinyString const&  get_last_operator() const{return last_operator;}

  void  clear();

};


}




#endif




