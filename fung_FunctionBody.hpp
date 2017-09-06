#ifndef FUNG_FunctionBody_HPP_INCLUDED
#define FUNG_FunctionBody_HPP_INCLUDED


#include"fung_statement.hpp"


namespace fung{


class
FunctionBody
{
  StatementList  statement_list;

public:
  FunctionBody(){}
  FunctionBody(StatementList&&  ls):
  statement_list(std::move(ls)){}


  void  reset(StatementList&&  ls){statement_list = std::move(ls);}

  StatementList const&  get_statement_list() const{return statement_list;}

};




}




#endif




