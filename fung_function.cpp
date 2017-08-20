#include"fung_function.hpp"
#include"fung_variable.hpp"




namespace fung{


Value
Function::
operator()(ArgumentList const&  args)
{
    for(auto&  stmt: body)
    {
        if(stmt == StatementKind::return_)
        {
          return stmt->evaluate(*this);
        }
    }


  return Value();
}




bool
Function::
test_argument_list(ArgumentList const&  args) const
{
    if(parameter_list.size() == args.size())
    {
      auto  a =           args.cbegin();
      auto  p = parameter_list.cbegin();

      auto  n = args.size();

        while(n--)
        {
        }


      return true;
    }


  return false;
}


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




