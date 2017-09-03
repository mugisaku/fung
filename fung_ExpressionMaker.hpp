#ifndef FUNG_ExpressionMaker_HPP_INCLUDED
#define FUNG_ExpressionMaker_HPP_INCLUDED


#include"fung_ExpressionNode.hpp"
#include"fung_cursor.hpp"
#include"fung_TinyString.hpp"
#include"fung_token.hpp"
#include<memory>


namespace fung{


class TinyString;


class
ExpressionMaker
{
  std::vector<char>       unop_stack;
  std::vector<Mnemonic>  binop_stack;

  std::vector<Expression>  output;

  bool  need_to_close;

  bool  last_is_operand;

  TinyString  last_operator;

  void   run_first_phase(Cursor&  cur              );
  void  step_first_phase(Cursor&  cur, Token&&  tok);

  static Expression   run_last_phase(std::vector<Expression>&&  src                    );
  static void        step_last_phase(std::vector<Expression>&   buf, Expression&&  expr);

  void  process_operator(Cursor&  cur, TinyString const&  o);

  void  push_operand(Expression&&  expr);

public:
  Expression  operator()(Cursor&  cur, char const*  onerr_msg="");

  TinyString const&  get_last_operator() const{return last_operator;}

  void  clear();

};


}




#endif




