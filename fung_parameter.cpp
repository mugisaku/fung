#include"fung_parameter.hpp"




namespace fung{


void
Parameter::
print() const
{
  printf("%s",name.data());

    if(expression)
    {
      printf(" = ");

      expression.print();
    }
}


}




