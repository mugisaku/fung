#include"fung_context.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{




bool
Context::
prepare_to_run(std::string const&  function_name)
{
  frame_stack.clear();

  auto&  c = global_space->find_constant(function_name);

    if(c)
    {
        if(c->value_kind == ValueKind::function)
        {
          returned_value = undefined;

          enter(function_name,*c->value->function);

          return true;
        }

      else
        {
          printf("%sは関数ではない",function_name.data());
        }
    }


  return false;
}


bool
Context::
run()
{
  interruption_flag = false;

    while(frame_stack.size())
    {
      auto&  frame = frame_stack.back();

      auto  stmt = frame.get_next_statement();

        if(stmt)
        {
          stmt->execute(*this);

            if(interruption_flag)
            {
              return false;
            }
        }

      else
        {
          frame_stack.pop_back();
        }
    }


  return true;
}


void
Context::
enter(std::string const&  fn_name, Function const&  fn)
{
    if(frame_stack.size() > 1000)
    {
      throw Error("入れ子が1000を越えた");
    }


  frame_stack.emplace_back(fn_name,fn);
}


void
Context::
leave()
{
    if(frame_stack.empty())
    {
      throw Error("フレームが無い");
    }


  frame_stack.pop_back();
}


void
Context::
entry(std::string const&  name, Expression const&  expr)
{
  frame_stack.back().append_variable(Variable(name,expr.evaluate(*this)));
}


void
Context::
entry(std::string const&  name, Value const&  val)
{
  frame_stack.back().append_variable(Variable(name,val));
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




