#include"fung_space.hpp"
#include"fung_variable.hpp"
#include"fung_function.hpp"




namespace fung{




void
Space::
append_variable(Variable&&  v)
{
  variable_list.emplace_back(std::move(v));
}


Variable const*
Space::
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
Space::
print() const
{
    for(auto&  v: variable_list)
    {
      v.print();
    }
}


}




