#include"fung_function.hpp"
#include"fung_variable.hpp"




namespace fung{


void
Function::
print() const
{
  printf("function(");

  auto        it = parameter_list.cbegin();
  auto const end = parameter_list.cend();

    if(it != end)
    {
      it++->print();

        while(it != end)
        {
          printf(",");

          it++->print();
        }
    }


  printf(")->%s\n{\n",return_type.data());

    for(auto&  stmt: body)
    {
      stmt.print();
    }


  printf("\n}\n\n");
}


}




