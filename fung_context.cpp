#include"fung_context.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{




void
Context::
push_frame(Frame&&  frame)
{
  frame_stack.emplace_back(std::move(frame));
}


void
Context::
pop_frame()
{
  frame_stack.pop_back();
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
  auto&  f = frame_stack.back();

    for(auto&  var: f.variable_list)
    {
        if(var.name == name)
        {
          return var.value;
        }
    }


  auto  fn = global_space->find_function(name);

    if(fn)
    {
      return Value(fn);
    }


  throw Error("関数%sで %s がみつからない",f.function.get_name().data(),name.data());
}


}




