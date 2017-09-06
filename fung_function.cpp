#include"fung_function.hpp"
#include"fung_context.hpp"
#include"fung_error.hpp"




namespace fung{




void
Function::
print() const
{
  printf("(");

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


  printf(")->%s\n{\n",Value::to_string(return_value_kind).data());

    if(body)
    {
        for(auto&  stmt: body->get_statement_list())
        {
          stmt.print();

          printf("\n");
        }
    }


  printf("}\n\n");
}


}




