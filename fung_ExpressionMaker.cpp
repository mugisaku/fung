#include"fung_ExpressionMaker.hpp"
#include"fung_token.hpp"
#include"fung_function.hpp"
#include"fung_identifier.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_variable.hpp"
#include"fung_error.hpp"
#include<cstdlib>




namespace fung{




namespace{


int
precedence(Mnemonic  mn)
{
  int  p = 80;

    switch(mn)
    {
  case(Mnemonic::acc): p -= 4;break;
  case(Mnemonic::der): p -= 4;break;

  case(Mnemonic::add    ): p -= 6;break;
  case(Mnemonic::sub    ): p -= 6;break;
  case(Mnemonic::mul    ): p -= 5;break;
  case(Mnemonic::div    ): p -= 5;break;
  case(Mnemonic::rem    ): p -= 5;break;
  case(Mnemonic::shl    ): p -= 7;break;
  case(Mnemonic::shr    ): p -= 7;break;
  case(Mnemonic::bit_and): p -= 10;break;
  case(Mnemonic::bit_xor): p -= 11;break;
  case(Mnemonic::bit_or ): p -= 12;break;

  case(Mnemonic::eq     ): p -= 9;break;
  case(Mnemonic::neq    ): p -= 9;break;
  case(Mnemonic::lt     ): p -= 8;break;
  case(Mnemonic::lteq   ): p -= 8;break;
  case(Mnemonic::gt     ): p -= 8;break;
  case(Mnemonic::gteq   ): p -= 8;break;
  case(Mnemonic::log_and): p -= 13;break;
  case(Mnemonic::log_or ): p -= 14;break;
  case(Mnemonic::cho    ): p -= 15;break;
  case(Mnemonic::eth    ): p -= 15;break;
  default:
      throw Error("不明なニーモニック");
    }


  return p;
}


bool
operator<(Mnemonic  a, Mnemonic  b)
{
  return precedence(a) < precedence(b);
}


Expression
read_either_expression(Cursor&  cur)
{
  auto  currec = cur;

  ExpressionMaker  mk;

  auto  l = mk(cur,"条件演算の第一式");

    if(!mk.get_last_operator().compare(':'))
    {
      throw Error(currec,"三項演算の\':\'がない");
    }


  auto  r = mk(cur,"条件演算の第二式");

  return Expression(ExpressionNode(Mnemonic::eth,std::move(l),std::move(r)));
}


Expression
read_postfix_expression(Cursor&  cur, Expression&&  expr)
{
  skip_spaces_and_newline(cur);

  ExpressionMaker  mk;

    if(*cur == '(')
    {
      cur += 1;

      ExpressionList  ls;

      skip_spaces_and_newline(cur);

        if(*cur != ')')
        {
          ls.emplace_back(mk(cur));

            while(!mk.get_last_operator().compare(')'))
            {
                if(!mk.get_last_operator().compare(','))
                {
                  throw Error("式リストがコンマ以外で区切られている %s",mk.get_last_operator().codes);
                }


              ls.emplace_back(mk(cur));
            }
        }


      auto  l = Expression(              (std::move(expr)));
      auto  r = Expression(ExpressionNode(std::move(  ls)));

      expr = Expression(ExpressionNode(Mnemonic::cal,std::move(l),std::move(r)));

      return read_postfix_expression(cur,std::move(expr));
    }

  else
    if(*cur == '[')
    {
      cur += 1;

      auto  l = Expression(std::move(expr));
      auto  r = Expression(        mk(cur));

        if(!mk.get_last_operator().compare(']'))
        {
          throw Error("\'[\'が\']\'で閉じていない");
        }


      expr = Expression(ExpressionNode(Mnemonic::sus,std::move(l),std::move(r)));

      return read_postfix_expression(cur,std::move(expr));
    }


  return std::move(expr);
}


}




void
ExpressionMaker::
push_operand(Expression&&  expr)
{
    while(unop_stack.size())
    {
      auto  c = unop_stack.back();
                unop_stack.pop_back();

      Expression  t(ExpressionNode(c == '-'? Mnemonic::neg    :
                                   c == '~'? Mnemonic::bit_not:
                                   c == '*'? Mnemonic::der:
                                   c == '!'? Mnemonic::log_not:Mnemonic::nop,std::move(expr)));

      expr = std::move(t);
    }


  output.emplace_back(std::move(expr));

  last_is_operand = true;
}


void
ExpressionMaker::
process_operator(Cursor&  cur, TinyString const&  o)
{
  last_operator = o;

    if(o.compare('('))
    {
      ExpressionMaker  mk;

      auto  expr = mk(cur,"式リスト");

        if(!mk.get_last_operator().compare(')'))
        {
          throw Error("\'(\'が\')\'で閉じていない");
        }


      expr = read_postfix_expression(cur,std::move(expr));

      push_operand(std::move(expr));
    }

  else
    if(o.compare('{'))
    {
      auto  stmtls = read_statement_list(cur,"関数内関数");

      auto  expr = Expression(ExpressionNode(std::move(stmtls)));
      
      expr = read_postfix_expression(cur,std::move(expr));

      push_operand(std::move(expr));
    }

  else
    if(o.compare('['))
    {
      throw Error("ここに'['は現れないはず");
    }

  else
    if(o.compare(';') ||
       o.compare(',') ||
       o.compare(')') ||
       o.compare(']') ||
       o.compare('}') ||
       o.compare(':'))
    {
      need_to_close = true;
    }

  else
    if(o.compare('?'))
    {
      auto  expr = read_either_expression(cur);

        while(binop_stack.size())
        {
          output.emplace_back(binop_stack.back());

          binop_stack.pop_back();
        }


      output.emplace_back(std::move(expr));

      output.emplace_back(Mnemonic::cho);

      need_to_close = true;
    }

  else
    if(!last_is_operand)
    {
           if(o.compare('!')){unop_stack.emplace_back('!');}
      else if(o.compare('~')){unop_stack.emplace_back('~');}
      else if(o.compare('-')){unop_stack.emplace_back('-');}
      else if(o.compare('*')){unop_stack.emplace_back('*');}

      last_is_operand = false;
    }

  else
    {
      Mnemonic  mn;

           if(o.compare('+'    )){mn = Mnemonic::add;}
      else if(o.compare('-','>')){mn = Mnemonic::ina;}
      else if(o.compare('-'    )){mn = Mnemonic::sub;}
      else if(o.compare('*'    )){mn = Mnemonic::mul;}
      else if(o.compare('/'    )){mn = Mnemonic::div;}
      else if(o.compare('%'    )){mn = Mnemonic::rem;}
      else if(o.compare('|','|')){mn = Mnemonic::log_or;}
      else if(o.compare('|'    )){mn = Mnemonic::bit_or;}
      else if(o.compare('&','&')){mn = Mnemonic::log_and;}
      else if(o.compare('&'    )){mn = Mnemonic::bit_and;}
      else if(o.compare('^'    )){mn = Mnemonic::bit_xor;}
      else if(o.compare('=','=')){mn = Mnemonic::eq;}
      else if(o.compare('!','=')){mn = Mnemonic::neq;}
      else if(o.compare('<','<')){mn = Mnemonic::shl;}
      else if(o.compare('<','=')){mn = Mnemonic::lteq;}
      else if(o.compare('<'    )){mn = Mnemonic::lt;}
      else if(o.compare('>','>')){mn = Mnemonic::shr;}
      else if(o.compare('>','=')){mn = Mnemonic::gteq;}
      else if(o.compare('>'    )){mn = Mnemonic::gt;}
      else if(o.compare('.'    )){mn = Mnemonic::acc;}
      else
        {
          throw Error(cur,"使えない二項演算子 %s",o.codes);
        }


        while(binop_stack.size() && (mn < binop_stack.back()))
        {
          output.emplace_back(binop_stack.back());

          binop_stack.pop_back();
        }


      binop_stack.emplace_back(mn);

      last_is_operand = false;
    }
}


void
ExpressionMaker::
step_first_phase(Cursor&  cur, Token&&  tok)
{
    if(tok == TokenKind::operator_)
    {
      process_operator(cur,tok->tiny_string);
    }

  else
    if(tok == TokenKind::integer)
    {
      Expression  expr(ExpressionNode(Value(static_cast<int>(tok->integer))));

      expr = read_postfix_expression(cur,std::move(expr));

      push_operand(std::move(expr));
    }

  else
    if(tok == TokenKind::string)
    {
      Expression  expr(ExpressionNode(Value(std::move(tok->string))));

      expr = read_postfix_expression(cur,std::move(expr));

      push_operand(std::move(expr));
    }

  else
    if(tok == TokenKind::identifier)
    {
      auto&  s = tok->string;

           if(s == "true" ){push_operand(Expression(ExpressionNode(Value( true))));}
      else if(s == "false"){push_operand(Expression(ExpressionNode(Value(false))));}
      else if(s == "list")
        {
          skip_spaces_and_newline(cur);

            if(*cur != '(')
            {
              throw Error(cur,"リストの中身が無い");
            }


          cur += 1;

          auto  expr = Expression(ExpressionNode(read_expression_list(cur)));

          expr = read_postfix_expression(cur,std::move(expr));

          push_operand(std::move(expr));
        }

      else
        {
          Identifier  id(std::move(s));

          Expression  expr(ExpressionNode(std::move(id)));

          expr = read_postfix_expression(cur,std::move(expr));

          push_operand(std::move(expr));
        }
    }

  else
    {
      tok.print();

      throw Error(cur,"式には使えない字句 %d",(int)tok.get_kind());
    }
}


void
ExpressionMaker::
run_first_phase(Cursor&  cur)
{
    while(*cur && !need_to_close)
    {
      auto  tok = read_token(cur);

      step_first_phase(cur,std::move(tok));
    }


    while(binop_stack.size())
    {
      output.emplace_back(binop_stack.back());

      binop_stack.pop_back();
    }
}


void
ExpressionMaker::
step_last_phase(std::vector<Expression>&  buf, Expression&&  e)
{
    if((*e == ExpressionNodeKind::value         ) ||
       (*e == ExpressionNodeKind::list          ) ||
       (*e == ExpressionNodeKind::statement_list) ||
       (*e == ExpressionNodeKind::operation     ) ||
       (*e == ExpressionNodeKind::identifier    ))
    {
      buf.emplace_back(std::move(e));
    }

  else
    if(*e == ExpressionNodeKind::operator_)
    {
        if(e->is_binary_operator())
        {
            if(buf.size() < 2)
            {
              throw Error(Cursor(),"オペランドが足りない %d",buf.size());
            }


          auto  r = Expression(std::move(buf.back()));

          buf.pop_back();

          auto  l = Expression(std::move(buf.back()));

          buf.pop_back();

          buf.emplace_back(ExpressionNode((*e)->mnemonic,std::move(l),std::move(r)));
        }
    }

  else
    {
      throw Error("不明な計算要素");
    }
}



Expression
ExpressionMaker::
run_last_phase(std::vector<Expression>&&  src)
{
  std::vector<Expression>  buf;

    for(auto&  e: src)
    {
      step_last_phase(buf,std::move(e));
    }


    if(buf.size() > 1)
    {
        for(auto&  e: buf)
        {
          e->print();
          printf("\n");
        }


      printf("\n");

      throw Error(Cursor(),"式生成の結果が不正 %d",buf.size());
    }


  return buf.empty()? Expression()
                    : std::move(buf.front());
}


Expression
ExpressionMaker::
operator()(Cursor&  cur, char const*  onerr_msg)
{
  Cursor  currec = cur;

  clear();

    try
    {
      run_first_phase(cur);
    }


    catch(Error&  e)
    {
      e.print();

      throw Error(currec,"%s 第一段階でエラー\n",onerr_msg);
    }


    try
    {
      return run_last_phase(std::move(output));
    }

    catch(Error&  e)
    {
      e.print();

      throw Error(currec,"%s 第二段階でエラー\n",onerr_msg);
    }
}


void
ExpressionMaker::
clear()
{
   unop_stack.clear();
  binop_stack.clear();

  output.clear();

  need_to_close   = false;
  last_is_operand = false;
}


}




