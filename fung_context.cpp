#include"fung_context.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{




Value
Context::
run(std::string const&  function_name, List&&  args)
{
  auto&  c = global_space->find_constant(function_name);

    if(!c)
    {
      throw Error("開始関数に指定した%sが見つからない",function_name.data());
    }


    if(c->value_kind != ValueKind::function)
    {
      throw Error("%sは関数ではない",function_name.data());
    }




  auto&  fn = *c->value->function;

  frame_stack.clear();

  interruption_flag = false;

  return call(function_name,fn,std::move(args));
}




void
Context::
check_depth() const
{
  constexpr int  limit = 1000;

    if(frame_stack.size() > limit)
    {
      throw Error("呼び出し深度が%dを越えた",limit);
    }
}


Value
Context::
call(std::string const&  fn_name, Function const&  fn, List&&  args)
{
  check_depth();


  frame_stack.emplace_back(fn_name,fn,std::move(args));

  auto  retval = start();

  frame_stack.pop_back();

  return std::move(retval);
}


Value
Context::
call(StatementList const&  stmtls)
{
  check_depth();


  frame_stack.emplace_back(stmtls);

  auto  retval = start();

  frame_stack.pop_back();

  return std::move(retval);
}


Value
Context::
start()
{
  auto&  xframe = frame_stack.back();

  Value  returned_value;

    for(;;)
    {
      auto  res = xframe.step(*this,returned_value);

        if(res == ExecutionResult::value_was_returned)
        {
          return std::move(returned_value);
        }

      else
        if(res == ExecutionResult::interrupted)
        {
          interruption_flag = true;

          break;
        }

      else
        if(res == ExecutionResult::error)
        {
          throw Error("コンテキスト実行エラー");
        }
    }


  return undefined;
}




void
Context::
entry(std::string const&  name, Expression const&  expr)
{
  frame_stack.back().append_variable(Variable(name,expr->evaluate(*this)));
}


void
Context::
entry(std::string const&  name, Value const&  val)
{
  frame_stack.back().append_variable(Variable(name,val));
}


void
Context::
entry(ParameterList const&  parals, List const&  ls)
{
  auto&  frm = frame_stack.back();

  auto&  fn_name = frm.get_function_name();

}




Value
Context::
find_value(std::string const&  name) const
{
  auto  var = frame_stack.back().find_variable(name);

    if(var)
    {
      return var->value;
    }


  return Value();
}


Value
Context::
operator[](std::string const&  name) const
{
  char const*  prefix = "無名関数";

    if(frame_stack.size())
    {
      auto&  f = frame_stack.back();

      prefix = f.get_function_name().data();

      auto  var = f.find_variable(name);

        if(var)
        {
          return var->value;
        }
    }


    if(global_space)
    {
      auto&  con = global_space->find_constant(name);

        if(con)
        {
          return con->get_value(global_space);
        }
    }


  throw Error("%sで %s がみつからない",prefix,name.data());
}


}




