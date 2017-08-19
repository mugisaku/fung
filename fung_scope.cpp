#include"fung_scope.hpp"
#include"fung_variable.hpp"
#include"fung_function.hpp"




namespace fung{




void
Scope::
append_variable(Variable&&  v)
{
  variable_list.emplace_back(std::move(v));
}


Variable const*
Scope::
find_variable(std::string const&  name) const
{
    for(auto&  v: variable_list)
    {
        if(v == name)
        {
          return &v;
        }
    }


  return nullptr;
}




void
Scope::
print() const
{
    for(auto&  v: variable_list)
    {
      v.print();
    }
}


}




