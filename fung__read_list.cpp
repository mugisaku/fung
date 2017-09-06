#include"fung.hpp"
#include"fung_identifier.hpp"
#include"fung_parameter.hpp"
#include"fung_statement.hpp"
#include"fung_ExpressionMaker.hpp"
#include"fung_error.hpp"




namespace fung{



namespace{


Parameter
read_parameter(Cursor&  cur)
{
  auto  type_name = read_identifier(cur);

  auto  vk = Value::to_kind(type_name);

  skip_spaces_and_newline(cur);

  auto  const c = *cur;

    if(!isident0(c))
    {
      throw Error(cur,"仮引数の変数名が無い");
    }


  auto  variable_name = read_identifier(cur);

  return Parameter(vk,std::move(variable_name));
}


Statement
read_statement(GlobalSpace&  sp, Cursor&  cur, std::string const&  fn_name)
{
  Cursor  currec = cur;

  Statement  stmt;

  skip_spaces_and_newline(cur);

    if(isident0(*cur))
    {
      auto  id = read_identifier(cur);

        if(id == "return")
        {
          ExpressionMaker  mk(sp);

          char  buf[256];

          snprintf(buf,sizeof(buf),"関数%s内return文:",fn_name.data());

          ReturnStatement  ret(mk(cur,buf));

          stmt = Statement(std::move(ret));
        }

      else
        if(id == "print")
        {
          ExpressionMaker  mk(sp);

          char  buf[256];

          snprintf(buf,sizeof(buf),"関数%s内print文:",fn_name.data());

          PrintStatement  prn(mk(cur,buf));

          stmt = Statement(std::move(prn));
        }

      else
        if(id == "interrupt")
        {
          stmt = Statement(InterruptStatement());
        }

      else
        if(id == "let")
        {
          skip_spaces_and_newline(cur);

            if(!isident0(*cur))
            {
              throw Error(currec,"let文で識別子が無い");
            }


          id = read_identifier(cur);

          skip_spaces_and_newline(cur);

            if(*cur != '=')
            {
              throw Error(currec,"let文で代入記号が無い");
            }


          cur += 1;


          ExpressionMaker  mk(sp);

          LetStatement  let(std::move(id),mk(cur));

          stmt = Statement(std::move(let));
        }
    }

  else
    if(*cur == ';')
    {
      cur += 1;
    }

  else
    {
      throw Error(currec,"文の途中で不明な文字");
    }


  return std::move(stmt);
}


}


ParameterList
read_parameter_list(Cursor&  cur)
{
  ParameterList  ls;

  skip_spaces_and_newline(cur);

  auto  const first_c = *cur;

    if(isident0(first_c))
    {
      ls.emplace_back(read_parameter(cur));

        for(;;)
        {
          skip_spaces_and_newline(cur);

          auto  const c = *cur;

            if(c == ')')
            {
              cur += 1;

              break;
            }

          else
            if(c == ',')
            {
              cur += 1;

              skip_spaces_and_newline(cur);

                if(isident0(*cur))
                {
                  ls.emplace_back(read_parameter(cur));
                }

              else
                {
                  throw Error(cur,"コンマの後に識別子が無い");
                }
            }

          else
            {
              throw Error(cur,"仮引数の途中に不明な文字");
            }
        }
    }

  else
    if(first_c != ')')
    {
      throw Error(cur,"仮引数のリストが閉じられていない");
    }

  else
    {
      cur += 1;
    }


  return std::move(ls);
}


ExpressionList
read_expression_list(GlobalSpace&  sp, Cursor&  cur)
{
  ExpressionList  ls;

    for(;;)
    {
      ExpressionMaker  mk(sp);

      auto  expr = mk(cur,"式リスト");

        if(expr)
        {
          ls.emplace_back(std::move(expr));
        }


      auto  o = mk.get_last_operator();

        if(!o.compare(','))
        {
          break;
        }
    }


  return std::move(ls);
}


StatementList
read_statement_list(GlobalSpace&  sp, Cursor&  cur, std::string const&  id)
{
  StatementList  ls;

    for(;;)
    {
      skip_spaces_and_newline(cur);

      auto  const c = *cur;

        if(c == '}')
        {
          cur += 1;

          break;
        }

      else
        if(!c)
        {
          throw Error(cur,"文リストの途中で終端文字");
        }

      else
        {
          auto  stmt = read_statement(sp,cur,id);

            if(stmt)
            {
              ls.emplace_back(std::move(stmt));
            }
        }
    }


  return std::move(ls);
}


}




