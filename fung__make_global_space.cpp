#include"fung_GlobalSpace.hpp"
#include"fung_token.hpp"
#include"fung_error.hpp"
#include"fung_function.hpp"
#include"fung_ExpressionMaker.hpp"




namespace fung{


namespace{


Statement
read_statement(Cursor&  cur)
{
  Statement  stmt;

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
              throw Error(cur,"let文で識別子が無い");
            }


          id = read_identifier(cur);

          skip_spaces_and_newline(cur);

            if(*cur != '=')
            {
              throw Error(cur,"let文で代入記号が無い");
            }


          cur += 1;


          ExpressionMaker  mk;

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
      throw Error(cur,"文の途中で不明な文字");
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
          auto  stmt = read_statement(cur);

            if(stmt)
            {
              body.emplace_back(std::move(stmt));
            }
        }
    }


  return std::move(body);
}


void
process_function(Cursor&  cur, std::unique_ptr<GlobalSpace>&  gsp)
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


          gsp->append_function(new Function(*gsp,std::move(id),std::move(parals),Value::to_kind(return_type),std::move(body)));
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
process_object(Cursor&  cur, std::unique_ptr<GlobalSpace>&  gsp)
{
}


}


std::unique_ptr<GlobalSpace>
make_global_space(Cursor&  cur)
{
  auto  gsp = std::make_unique<GlobalSpace>();

    while(*cur)
    {
      auto  tok = read_token(cur);

        if(tok)
        {
            if(tok == TokenKind::identifier)
            {
              auto&  id = tok->string;

                if(id == "function"){process_function(cur,gsp);}
              else
                {
                  auto  fn = gsp->find_function(id);

                    if(fn)
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


  return std::move(gsp);
}


}




