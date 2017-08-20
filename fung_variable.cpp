#include"fung_variable.hpp"
#include"fung_function.hpp"
#include"fung_space.hpp"




namespace fung{


Value
Variable::
get_value(Space const&  sp) const
{
    if(function)
    {
      return Value(function.get());
    }

  else
    {
      return expression.evaluate(sp);
    }
}


void
Variable::
print() const
{
  printf("%s",name.data());

    if(function)
    {
      printf(" = ");

      function->print();
    }

  else
    if(expression)
    {
      printf(" = ");

      expression.print();
    }
}


}




