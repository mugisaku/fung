#include"fung_space.hpp"
#include"fung_function.hpp"
#include"fung_variable.hpp"
#include"fung_value.hpp"




namespace fung{



Space::
Space(Space&  parent_):
global(parent_.global),
parent(&parent_)
{
}




std::unique_ptr<Constant> const&
Space::
find_constant(std::string const&  name) const
{
    for(auto&  v: constant_list)
    {
        if(v && (v->name == name))
        {
          return v;
        }
    }


  static std::unique_ptr<Constant>  null;

  return(this != parent)? parent->find_constant(name):null;
}


void
Space::
print() const
{
    for(auto&  c: constant_list)
    {
      c->print();

      printf("\n");
    }
}




}




