#include"fung_statement.hpp"
#include"fung_Expression.hpp"
#include"fung_function.hpp"
#include"fung_context.hpp"




namespace fung{




Statement::
Statement(InterruptStatement  intstmt):
kind(StatementKind::interrupt)
{
}


Statement::
Statement(LetStatement&&  bin):
kind(StatementKind::let),
identifier(std::move(bin.identifier)),
expression(std::move(bin.expression))
{
}


Statement::
Statement(PrintStatement&&  prn):
kind(StatementKind::print),
expression(std::move(prn.expression))
{
}


Statement::
Statement(ReturnStatement&&  ret):
kind(StatementKind::return_),
expression(std::move(ret.expression))
{
}


 

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
/*
    switch(kind)
    {
  case(StatementKind::null):
      break;
  case(StatementKind::interrupt):
      break;
  case(StatementKind::print):
      break;
  case(StatementKind::let):
      break;
  case(StatementKind::return_):
      break;
    }
*/


  expression.reset();

  kind = StatementKind::null;
}




bool
Statement::
execute(Context&  ctx) const
{
    switch(kind)
    {
  case(StatementKind::null):
      break;
  case(StatementKind::interrupt):
      ctx.set_interruption_flag();
      break;
  case(StatementKind::print):
      printf("[print stmt] ");

      expression->print();

      printf(" -> ");

      expression->evaluate(ctx).print();

      printf("\n");
      break;
  case(StatementKind::let):
      ctx.entry(identifier,expression);
      break;
  case(StatementKind::return_):
      ctx.hold_returned_value(expression->evaluate(ctx));

      return false;
      break;
    }


  return true;
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
  case(StatementKind::interrupt):
      printf("interrupt");
      break;
  case(StatementKind::print):
      printf("print  ");

      expression->print();
      break;
  case(StatementKind::let):
      printf("let  %s = ",identifier.data());

      expression->print();
      break;
  case(StatementKind::return_):
      printf("return ");
      expression->print();
      break;
    }


  printf(";");
}


}




