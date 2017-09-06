#include"fung_ExecutionFrame.hpp"
#include"fung_function.hpp"
#include"fung_context.hpp"
#include"fung_variable.hpp"




namespace fung{




ExecutionFrame::
ExecutionFrame(std::string const&  fn_name, Function const&  fn, List&&  valls):
function_name(fn_name),
function_body(*fn.get_body()),
return_value_kind(fn.get_return_value_kind()),
current(fn.get_body()->get_statement_list().cbegin())
{
  auto&  parals = fn.get_parameter_list();

    if(parals.size() != valls.length())
    {
      throw Error("%s 引数の数が一致しない",function_name.data());
    }


  auto  it = valls.begin();

    for(auto&  para: parals)
    {
      auto  v = std::move(*it++);

        if(para.get_value_kind() != v.get_kind())
        {
          v = v.convert_to(para.get_value_kind());
          
            if(para.get_value_kind() != v.get_kind())
            {
              auto&  a = Value::to_string(para.get_value_kind());
              auto&  b = Value::to_string(         v.get_kind());

              throw Error("ExecutionFrame: %s, %s <- %s 引数の型が一致しないか、変換不可",fn_name.data(),a.data(),b.data());
            }
        }


      argument_list.emplace_back(std::string(para.get_name()),std::move(v));
    }
}


namespace{
std::string  const unnamed("無名関数");
}


ExecutionFrame::
ExecutionFrame(FunctionBody const&  body, ExecutionFrame const*  parent_):
function_name(unnamed),
function_body(body),
current(body.get_statement_list().cbegin()),
parent(parent_)
{
}




void
ExecutionFrame::
update_argument_list(List&&  valls)
{
  auto  it = valls.begin();

    for(auto&  v: argument_list)
    {
      v.value = std::move(*it++);
    }
}


void
ExecutionFrame::
append_variable(Variable&&  var)
{
   for(auto&  v: variable_list)
   {
       if(v.name == var.name)
       {
         throw Error("すでに%sという名前の変数がある",var.name.data());
       }
   }


  variable_list.emplace_back(std::move(var));
}


Variable const*
ExecutionFrame::
find_variable(std::string const&  name) const
{
    for(auto&  var: variable_list)
    {
        if(var.name == name)
        {
          return &var;
        }
    }


    for(auto&  var: argument_list)
    {
        if(var.name == name)
        {
          return &var;
        }
    }


  return parent? parent->find_variable(name):nullptr;
}


ExecutionResult
ExecutionFrame::
step(Context&  ctx, Value&  retval_buf)
{
    if(current == function_body.get_statement_list().cend())
    {
      return ExecutionResult::error;
    }


  auto&  stmt = *current++;

    switch(stmt.get_kind())
    {
  case(StatementKind::null):
      break;
  case(StatementKind::interrupt):
      return ExecutionResult::interrupted;
      break;
  case(StatementKind::print):
      printf("[print stmt] ");

      stmt.get_expression()->print();

      printf(" -> ");

      stmt.get_expression()->evaluate(ctx).print();

      printf("\n");
      break;
  case(StatementKind::let):
      ctx.entry(stmt.get_identifier(),stmt.get_expression());
      break;
  case(StatementKind::return_):
      retval_buf = stmt.get_expression()->evaluate(ctx);

        if(retval_buf == ValueKind::tail_calling)
        {
//          printf("末尾再帰最適化!\n");

          current = function_body.get_statement_list().cbegin();

          update_argument_list(std::move(retval_buf->list));


          return ExecutionResult::no_value_was_returned;
        }


      auto  k = return_value_kind;

        if(k != ValueKind::null)
        {
            if(retval_buf.get_kind() != k)
            {
              retval_buf = retval_buf.convert_to(k);

                if(retval_buf.get_kind() != k)
                {
                  auto&  a = Value::to_string(k);
                  auto&  b = Value::to_string(retval_buf.get_kind());

                  throw Error("返り値の型が一致しないか、変換不可 %s <- %s",a.data(),b.data());
                }
            }
        }


      return ExecutionResult::value_was_returned;
      break;
    }


  return ExecutionResult::no_value_was_returned;
}


}




