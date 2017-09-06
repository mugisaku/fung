#include"fung_context.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{




Value
Context::
run(std::string const&  function_name, List&&  args)
{
  auto&  v = global_space->find(function_name);

    if(!v)
    {
      throw Error("開始関数に指定した%sが見つからない",function_name.data());
    }


    if(v->value_kind != ValueKind::function)
    {
      throw Error("%sは関数ではない",function_name.data());
    }




  auto&  fn = *v->value->function;

  frame_stack.clear();

  stack_count = 0;

  interruption_flag = false;

  return call(function_name,fn,std::move(args));
}




void
Context::
check_depth() const
{
  constexpr int  limit = 4000;

    if(stack_count > limit)
    {
      throw Error("呼び出し深度が%dを越えた",limit);
    }


//  printf("%zd\n",stack_count);
}


Value
Context::
call(std::string const&  fn_name, Function const&  fn, List&&  args)
{
  check_depth();


  frame_stack.emplace_front(fn_name,fn,std::move(args));

  ++stack_count;

  auto  retval = start();

  frame_stack.pop_front();

  --stack_count;

  return std::move(retval);
}


Value
Context::
call(FunctionBody const&  fnbody)
{
  check_depth();


  frame_stack.emplace_front(fnbody,&frame_stack.front());

  ++stack_count;

  auto  retval = start();

  frame_stack.pop_front();

  --stack_count;

  return std::move(retval);
}


Value
Context::
start()
{
  auto&  xframe = frame_stack.front();

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
  auto  v = expr->evaluate(*this);

/*
printf("[entry] ");
v.print();
printf(" <- ");
expr->print();
printf("\n");
*/

  frame_stack.front().append_variable(Variable(std::string(name),std::move(v)));
}


void
Context::
entry(std::string const&  name, Value const&  val)
{
  frame_stack.front().append_variable(Variable(std::string(name),Value(val)));
}




Value
Context::
find_value(std::string const&  name) const
{
  auto  var = frame_stack.front().find_variable(name);

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

    if(stack_count)
    {
      auto&  f = frame_stack.front();

      prefix = f.get_function_name().data();

      auto  var = f.find_variable(name);

        if(var)
        {
          return var->value;
        }
    }


    if(global_space)
    {
      auto&  v = global_space->find(name);

        if(v)
        {
          return v->get_value(global_space);
        }
    }


  throw Error("%sで %s がみつからない",prefix,name.data());
}


}




