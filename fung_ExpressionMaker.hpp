#ifndef FUNG_ExpressionMaker_HPP_INCLUDED
#define FUNG_ExpressionMaker_HPP_INCLUDED


#include"fung_ExpressionNode.hpp"
#include"fung_cursor.hpp"
#include"fung_TinyString.hpp"
#include"fung_token.hpp"
#include<memory>


namespace fung{


class TinyString;
class GlobalSpace;


class
ExpressionMaker
{
  GlobalSpace&  space;

  std::vector<char>       unop_stack;
  std::vector<Mnemonic>  binop_stack;

  std::vector<Expression>  output;

  char  close_character;
  char   last_character;

  bool  last_is_operand;

  void   run_first_phase(Cursor&  cur              );
  void  step_first_phase(Cursor&  cur, Token&&  tok);

  static Expression   run_last_phase(std::vector<Expression>&&  src                    );
  static void        step_last_phase(std::vector<Expression>&   buf, Expression&&  expr);

  void  process_operator(Cursor&  cur, TinyString const&  o);

  void  push_operand(Expression&&  expr);

public:
  ExpressionMaker(GlobalSpace&  gsp):
  space(gsp){}

  Expression  operator()(Cursor&  cur, char  close_character_, char const*  onerr_msg="");

  char  get_last_character() const{return last_character;}

  void  clear();

};


}




#endif




