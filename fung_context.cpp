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

  auto  fn = global_space->find_function(function_name);

    if(fn)
    {
      returned_value = undefined;

      enter(*fn);

      return true;
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
enter(Function const&  fn)
{
    if(frame_stack.size() > 1000)
    {
      throw Error("入れ子が1000を越えた");
    }


  frame_stack.emplace_back(fn);
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


  auto  fn = global_space->find_function(name);

  return fn? Value(fn):Value();
}


Value
Context::
operator[](std::string const&  name) const
{
    if(frame_stack.empty())
    {
      throw Error("フレームが無い");
    }


  auto&  f = frame_stack.back();

  auto  var = f.find_variable(name);

    if(var)
    {
      return var->value;
    }


    if(global_space)
    {
      auto  fn = global_space->find_function(name);

        if(fn)
        {
          return Value(fn);
        }
    }


  throw Error("関数%sで %s がみつからない",f.get_function().get_name().data(),name.data());
}


}




