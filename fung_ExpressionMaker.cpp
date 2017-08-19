#include"fung_ExpressionMaker.hpp"
#include"fung_token.hpp"
#include"fung_function.hpp"
#include"fung_identifier.hpp"
#include<cstdlib>




namespace fung{




namespace{


int
precedence(Mnemonic  mn)
{
  int  p = 80;

    switch(mn)
    {
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
  ExpressionMaker  mk;

  auto  l = new Expression(mk(cur));

    if(mk.get_last_operator().compare(':'))
    {
      throw Error("三項演算の\':\'がない");
    }


  auto  r = new Expression(mk(cur));

  return Expression(Mnemonic::eth,l,r);
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

      Expression  t(c == '-'? Mnemonic::neg    :
                    c == '~'? Mnemonic::bit_not:
                    c == '!'? Mnemonic::log_not:Mnemonic::nop,new Expression(std::move(expr)));

      expr = std::move(t);
    }


  output.emplace_back(std::move(expr));

  last_is_operand = true;
}


bool
ExpressionMaker::
process_operator(Cursor&  cur, TinyString const&  o)
{
    if(o.compare('('))
    {
      ExpressionMaker  mk;

      push_operand(mk(cur));
    }

  else
    if(o.compare(';') ||
       o.compare(')') ||
       o.compare(':'))
    {
      last_operator = o;

      return false;
    }

  else
    if(!last_is_operand)
    {
           if(o.compare('!')){unop_stack.emplace_back('!');}
      else if(o.compare('~')){unop_stack.emplace_back('~');}
      else if(o.compare('-')){unop_stack.emplace_back('-');}

      last_is_operand = false;
    }

  else
    {
      Mnemonic  mn;

           if(o.compare('+'    )){mn = Mnemonic::add;}
      else if(o.compare('-'    )){mn = Mnemonic::sub;}
      else if(o.compare('*'    )){mn = Mnemonic::mul;}
      else if(o.compare('/'    )){mn = Mnemonic::div;}
      else if(o.compare('%'    )){mn = Mnemonic::rem;}
      else if(o.compare('|'    )){mn = Mnemonic::bit_or;}
      else if(o.compare('&'    )){mn = Mnemonic::bit_and;}
      else if(o.compare('^'    )){mn = Mnemonic::bit_xor;}
      else if(o.compare('=','=')){mn = Mnemonic::eq;}
      else if(o.compare('!','=')){mn = Mnemonic::neq;}
      else if(o.compare('<'    )){mn = Mnemonic::lt;}
      else if(o.compare('<','=')){mn = Mnemonic::lteq;}
      else if(o.compare('>'    )){mn = Mnemonic::gt;}
      else if(o.compare('>','=')){mn = Mnemonic::gteq;}
      else if(o.compare('<','<')){mn = Mnemonic::shl;}
      else if(o.compare('>','>')){mn = Mnemonic::shr;}
      else if(o.compare('&','&')){mn = Mnemonic::log_and;}
      else if(o.compare('|','|')){mn = Mnemonic::log_or;}
      else if(o.compare('?'    ))
        {
          mn = Mnemonic::cho;


          auto  expr = read_either_expression(cur);

          output.emplace_back(std::move(expr));

          last_is_operand = true;
        }

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


  return true;
}


Expression
ExpressionMaker::
operator()(Cursor&  cur)
{
  clear();

    while(*cur)
    {
      auto  tok = read_token(cur);

        if(tok == TokenKind::operator_)
        {
            if(!process_operator(cur,tok->tiny_string))
            {
              break;
            }
        }

      else
        if(tok == TokenKind::integer)
        {
          Expression  expr(Value(static_cast<int>(tok->integer)));

          push_operand(std::move(expr));
        }

      else
        if(tok == TokenKind::string)
        {
          Expression  expr(Value(std::move(tok->string)));

          push_operand(std::move(expr));
        }

      else
        if(tok == TokenKind::identifier)
        {
          auto&  s = tok->string;

               if(s == "true" ){push_operand(Expression(Value( true)));}
          else if(s == "false"){push_operand(Expression(Value(false)));}
          else
            {
              Identifier  id(std::move(s));

              Expression  expr(std::move(id));

              push_operand(std::move(expr));
            }
        }

      else
        {
          tok.print();

          throw Error(cur,"式には使えない字句 %d",(int)tok.get_kind());
        }
    }


    while(binop_stack.size())
    {
      output.emplace_back(binop_stack.back());

      binop_stack.pop_back();
    }


  std::vector<Expression>  calc;

    for(auto&  e: output)
    {
        if((e == ExpressionKind::value     ) ||
           (e == ExpressionKind::identifier))
        {
          calc.emplace_back(std::move(e));
        }

      else
        if(e == ExpressionKind::operator_)
        {
            if(e.is_binary_operator())
            {
                if(calc.size() < 2)
                {
                  throw Error(Cursor(),"オペランドが足りない %d",calc.size());
                }


              auto  r = new Expression(std::move(calc.back()));

              calc.pop_back();

              auto  l = new Expression(std::move(calc.back()));

              calc.pop_back();

              calc.emplace_back(e->mnemonic,l,r);
            }
        }

      else
        {
          throw Error("不明な計算要素");
        }
    }


    if(calc.size() != 1)
    {
        for(auto&  e: calc)
        {
          e.print();
      printf("\n");
        }


      printf("\n");

      throw Error(Cursor(),"演算結果が不正 %d",calc.size());
    }


  return std::move(calc.front());
}


void
ExpressionMaker::
clear()
{
   unop_stack.clear();
  binop_stack.clear();

  output.clear();

  last_is_operand = false;
}


}




