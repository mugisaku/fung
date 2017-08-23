#include"fung_context.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{




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
  frame_stack.back().append(Variable(name,expr.evaluate(*this)));
}


void
Context::
entry(std::string const&  name, Value const&  val)
{
  frame_stack.back().append(Variable(name,val));
}


Value
Context::
find_value(std::string const&  name) const
{
    for(auto&  var: frame_stack.back().variable_list)
    {
        if(var.name == name)
        {
          return var.value;
        }
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

    for(auto&  var: f.variable_list)
    {
        if(var.name == name)
        {
          return var.value;
        }
    }


    if(global_space)
    {
      auto  fn = global_space->find_function(name);

        if(fn)
        {
          return Value(fn);
        }
    }


  throw Error("関数%sで %s がみつからない",f.function.get_name().data(),name.data());
}


}




