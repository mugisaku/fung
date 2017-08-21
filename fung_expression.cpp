#include"fung_expression.hpp"
#include"fung_function.hpp"
#include"fung_cursor.hpp"
#include<new>




namespace fung{




Expression::
Expression(Identifier&&  id):
kind(ExpressionKind::identifier)
{
  new(&data) Identifier(std::move(id));
}


Expression::
Expression(Value&&  v):
kind(ExpressionKind::value)
{
  new(&data) Value(std::move(v));
}


Expression::
Expression(Mnemonic  mn, Expression*  l, Expression*  r):
kind(l? ExpressionKind::operation:ExpressionKind::operator_),
left(l),
right(r)
{
  data.mnemonic = mn;
}




Expression&
Expression::
operator=(Expression const&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

    switch(kind)
    {
  case(ExpressionKind::null):
      break;
  case(ExpressionKind::operator_):
  case(ExpressionKind::operation):
      data.mnemonic = rhs.data.mnemonic;
      break;
  case(ExpressionKind::value):
      new(&data) Value(rhs.data.value);
      break;
  case(ExpressionKind::identifier):
      new(&data) Identifier(rhs.data.identifier);
      break;
    }


  left  = rhs.left ? new Expression(*rhs.left ):nullptr;
  right = rhs.right? new Expression(*rhs.right):nullptr;

  return *this;
}


Expression&
Expression::
operator=(Expression&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(ExpressionKind::null):
      break;
  case(ExpressionKind::operator_):
  case(ExpressionKind::operation):
      data.mnemonic = rhs.data.mnemonic;
      break;
  case(ExpressionKind::value):
      new(&data) Value(std::move(rhs.data.value));
      break;
  case(ExpressionKind::identifier):
      new(&data) Identifier(std::move(rhs.data.identifier));
      break;
    }


  std::swap(left ,rhs.left );
  std::swap(right,rhs.right);

  return *this;
}




void
Expression::
clear()
{
    switch(kind)
    {
  case(ExpressionKind::null):
  case(ExpressionKind::operator_):
  case(ExpressionKind::operation):
      break;
  case(ExpressionKind::value):
      data.value.~Value();
      break;
  case(ExpressionKind::identifier):
      data.identifier.~Identifier();
      break;
    }


  delete left          ;
         left = nullptr;

  delete right          ;
         right = nullptr;

  kind = ExpressionKind::null;
}


bool
Expression::
is_unary_operator() const
{
  auto&  mn = data.mnemonic;

  return((mn == Mnemonic::neg    ) ||
         (mn == Mnemonic::bit_not) ||
         (mn == Mnemonic::log_not));
}


bool
Expression::
is_binary_operator() const
{
  auto&  mn = data.mnemonic;

  return((mn == Mnemonic::add    ) ||
         (mn == Mnemonic::sub    ) ||
         (mn == Mnemonic::mul    ) ||
         (mn == Mnemonic::div    ) ||
         (mn == Mnemonic::rem    ) ||
         (mn == Mnemonic::shl    ) ||
         (mn == Mnemonic::shr    ) ||
         (mn == Mnemonic::bit_or ) ||
         (mn == Mnemonic::bit_and) ||
         (mn == Mnemonic::bit_xor) ||
         (mn == Mnemonic::log_or ) ||
         (mn == Mnemonic::log_and) ||
         (mn == Mnemonic::eq     ) ||
         (mn == Mnemonic::neq    ) ||
         (mn == Mnemonic::lt     ) ||
         (mn == Mnemonic::lteq   ) ||
         (mn == Mnemonic::gt     ) ||
         (mn == Mnemonic::gteq   ) ||
         (mn == Mnemonic::cho    ) ||
         (mn == Mnemonic::eth    ) ||
         (mn == Mnemonic::sus    ) ||
         (mn == Mnemonic::cal    ));
}


bool
Expression::
is_operand() const
{
  return((kind == ExpressionKind::operation ) ||
         (kind == ExpressionKind::value     ) ||
         (kind == ExpressionKind::identifier));
}


void
Expression::
print() const
{
    switch(kind)
    {
  case(ExpressionKind::null):
      printf("NULL ");
      break;
  case(ExpressionKind::operator_):
      print_mnemonic(data.mnemonic);
      break;
  case(ExpressionKind::operation):
        if(is_unary_operator())
        {
          print_mnemonic(data.mnemonic);

          if(left){left->print();}
        }

      else
        {
          printf("(");

          if(left){left->print();}

          print_mnemonic(data.mnemonic);

          if(right){right->print();}

          printf(")");
        }
      break;
  case(ExpressionKind::value):
      data.value.print();
      break;
  case(ExpressionKind::identifier):
      printf("%s",data.identifier->data());
      break;
    }
}


void
Expression::
print_mnemonic(Mnemonic  mn)
{
  char const*  s = "";

    switch(mn)
    {
  case(Mnemonic::nop): s = "_";break;
  case(Mnemonic::add): s = "+";break;
  case(Mnemonic::sub): s = "-";break;
  case(Mnemonic::mul): s = "*";break;
  case(Mnemonic::div): s = "/";break;
  case(Mnemonic::rem): s = "%";break;
  case(Mnemonic::shl): s = "<<";break;
  case(Mnemonic::shr): s = ">>";break;
  case(Mnemonic::bit_and): s = "&";break;
  case(Mnemonic::bit_or): s = "|";break;
  case(Mnemonic::bit_xor): s = "^";break;
  case(Mnemonic::bit_not): s = "~";break;
  case(Mnemonic::eq): s = "==";break;
  case(Mnemonic::neq): s = "!=";break;
  case(Mnemonic::lt): s = "<";break;
  case(Mnemonic::lteq): s = "<=";break;
  case(Mnemonic::gt): s = ">";break;
  case(Mnemonic::gteq): s = ">=";break;
  case(Mnemonic::log_or): s = "||";break;
  case(Mnemonic::log_and): s = "&&";break;
  case(Mnemonic::log_not): s = "!";break;
  case(Mnemonic::neg): s = "-";break;
  case(Mnemonic::cho): s = "?";break;
  case(Mnemonic::eth): s = ":";break;
  case(Mnemonic::sus): s = "";break;
  case(Mnemonic::cal): s = "";break;
    }


  printf("%s",s);
}


}




