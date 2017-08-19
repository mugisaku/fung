#include"fung_variable.hpp"
#include"fung_function.hpp"
#include"fung_scope.hpp"




namespace fung{


Value
Variable::
get_value(Scope const&  scope) const
{
    if(function)
    {
      return Value(function.get());
    }

  else
    {
      return expression.evaluate(scope);
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




