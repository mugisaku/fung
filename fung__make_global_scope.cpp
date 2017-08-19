#include"fung_scope.hpp"
#include"fung_token.hpp"
#include"fung_cursor.hpp"
#include"fung_variable.hpp"
#include"fung_alias.hpp"
#include"fung_object.hpp"
#include"fung_function.hpp"
#include"fung_ExpressionMaker.hpp"




namespace fung{


namespace{


Statement
read_statement(Cursor&  cur)
{
  Statement  stmt;

    for(;;)
    {
      skip_spaces_and_newline(cur);

        if(isident0(*cur))
        {
          auto  id = read_identifier(cur);

            if(id == "return")
            {
              ExpressionMaker  mk;

              ReturnStatement  ret(mk(cur));

              stmt = Statement(std::move(ret));
            }
        }

      else
        if(*cur == '}')
        {
          break;
        }

      else
        {
          throw Error(cur,"関数本体の途中で不明な文字");
        }
    }


  return std::move(stmt);
}


FunctionBody
read_function_body(Cursor&  cur)
{
  FunctionBody  body;

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
          throw Error(cur,"関数本体の途中で終端文字");
        }

      else
        {
          body.emplace_back(read_statement(cur));
        }
    }


  return std::move(body);
}


void
process_function(Cursor&  cur, std::unique_ptr<Scope>&  scp)
{
  auto  next_tok = read_token(cur);

  std::string  id;

    if(next_tok == TokenKind::identifier)
    {
      id = std::move(next_tok->string);

      next_tok = read_token(cur);
    }


    if(next_tok == TokenKind::operator_)
    {
      auto&  op = next_tok->tiny_string;

        if(op.compare('('))
        {
          auto  parals = read_parameter_list(cur);

          skip_spaces_and_newline(cur);

            if(!cur.compare('-','>'))
            {
              throw Error(cur,"返り値を指す演算子が無い");
            }


          cur += 2;

          skip_spaces_and_newline(cur);

            if(!isident0(*cur))
            {
              throw Error(cur,"返り値が無い");
            }


          std::string  return_type = read_identifier(cur);

          skip_spaces_and_newline(cur);

          FunctionBody  body;

            if(*cur == '{')
            {
              cur += 1;

              body = read_function_body(cur);
            }


          auto  fn = new Function(std::move(parals),std::move(return_type),std::move(body));

          scp->append_variable(Variable(std::move(id),fn));
        }

      else
        {
          throw Error("関数のパラメーターのリストが無い");
        }
    }

  else
    {
      throw Error("不明な字句が続いている");
    }
}


void
process_object(Cursor&  cur, std::unique_ptr<Scope>&  scp)
{
}


}


std::unique_ptr<Scope>
make_global_scope(Cursor&  cur)
{
  auto  gscp = std::make_unique<Scope>();

    while(*cur)
    {
      auto  tok = read_token(cur);

        if(tok)
        {
            if(tok == TokenKind::identifier)
            {
              auto&  id = tok->string;

                if(id == "function"){process_function(cur,gscp);}
              else
                {
                  auto  v = gscp->find_variable(id);

                    if(v)
                    {
                    }

                  else
                    {
                      tok.print();

                      throw Error("不明な字句が続いている");
                    }
                }
            }

          else
            {
              tok.print();

              throw Error("不明な字句から始まっている");
            }
        }
    }


  return std::move(gscp);
}


}




