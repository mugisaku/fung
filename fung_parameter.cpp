#include"fung_parameter.hpp"




namespace fung{


void
Parameter::
print() const
{
  printf("%s  %s",Value::to_string(value_kind).data(),name.data());

    if(expression)
    {
      printf(" = ");

      expression.print();
    }
}


}




