#include"fung_GlobalSpace.hpp"
#include"fung_token.hpp"
#include"fung_error.hpp"
#include"fung_function.hpp"
#include"fung_ExpressionMaker.hpp"




namespace fung{


namespace{


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

          StatementList  stmtls;

          auto  fnbody = new FunctionBody;

          gsp->append_function_body(fnbody);

            if(*cur == '{')
            {
              cur += 1;

              fnbody->reset(read_statement_list(*gsp,cur,id));
            }


          auto  fn = new Function(std::move(parals),Value::to_kind(return_type),fnbody);

          gsp->append_function(std::move(id),fn);
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
process_constant(ValueKind  k, Cursor&  cur, std::unique_ptr<GlobalSpace>&  gsp)
{
  skip_spaces_and_newline(cur);

    if(!isident0(*cur))
    {
      throw Error(cur,"定数の識別子がない");
    }


  auto  id = read_identifier(cur);

  skip_spaces_and_newline(cur);

    if(*cur != '(')
    {
      throw Error(cur,"定数の定義がない");
    }


  cur += 1;

  auto  exprls = read_expression_list(*gsp,cur);


  gsp->append_variable(new Variable(k,std::move(id),std::move(exprls)));
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
                if((id == "integer") ||
                   (id == "boolean") ||
                   (id ==  "string") ||
                   (id ==    "list"))
                {
                  process_constant(Value::to_kind(id),cur,gsp);
                }

              else
                {
                  tok.print();

                  throw Error("不明な字句が続いている");
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




