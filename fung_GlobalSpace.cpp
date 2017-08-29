#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"




namespace fung{




GlobalSpace::
GlobalSpace(): Space(*this)
{
}




Function const&
GlobalSpace::
append_function(Function*  fn)
{
  function_list.emplace_back(fn);

  return *function_list.back();
}


Function const*
GlobalSpace::
find_function(std::string const&  name) const
{
    for(auto&  fn: function_list)
    {
        if(fn->get_name() == name)
        {
          return fn.get();
        }
    }


  return nullptr;
}


void
GlobalSpace::
print() const
{
    for(auto&  fn: function_list)
    {
      fn->print();
    }
}


}




