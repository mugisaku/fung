#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"




namespace fung{




GlobalSpace::
GlobalSpace(): Space(*this)
{
}




void
GlobalSpace::
append_function(std::string&&  name, Function*  fn)
{
  function_list.emplace_back(fn);

  auto&  bk = function_list.back();

  append_constant(new Constant(std::string(name),Value(*bk)));
}


}




