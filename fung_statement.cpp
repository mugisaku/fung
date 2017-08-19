#include"fung_statement.hpp"
#include"fung_variable.hpp"
#include"fung_function.hpp"




namespace fung{




Statement::Statement(ReturnStatement&&  ret): kind(StatementKind::return_), expression(std::move(ret.expression)){}


 

Statement&
Statement::
operator=(Statement const&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

  identifier = rhs.identifier;
  expression = rhs.expression;

  return *this;
}


Statement&
Statement::
operator=(Statement&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

  identifier = std::move(rhs.identifier);
  expression = std::move(rhs.expression);

  return *this;
}


void
Statement::
clear()
{
    switch(kind)
    {
  case(StatementKind::null):
      break;
  case(StatementKind::bind):
      break;
  case(StatementKind::return_):
      break;
    }


  kind = StatementKind::null;
}




void
Statement::
print() const
{
    switch(kind)
    {
  case(StatementKind::null):
      printf("NULL_STMT ");
      break;
  case(StatementKind::bind):
      printf("bind %s = ",identifier.data());

      expression.print();
      break;
  case(StatementKind::return_):
      printf("return ");
      expression.print();
      break;
    }


  printf(";");
}


}




