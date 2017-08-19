#include"fung_value.hpp"
#include"fung_function.hpp"
#include"fung_variable.hpp"




namespace fung{




Value::Value(int  i): kind(ValueKind::integer){data.integer = i;}
Value::Value(bool  b): kind(ValueKind::boolean){data.boolean = b;}
Value::Value(Function const*  fn): kind(ValueKind::function){data.function = fn;}
Value::Value(std::string&&  s): kind(ValueKind::string){new(&data) std::string(std::move(s));}




Value&
Value::
operator=(Value const&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      data.boolean = rhs.data.boolean;
      break;
  case(ValueKind::integer):
      data.integer = rhs.data.integer;
      break;
  case(ValueKind::string):
      new(&data) std::string(rhs.data.string);
      break;
  case(ValueKind::function):
      data.function = rhs.data.function;
      break;
    }


  return *this;
}


Value&
Value::
operator=(Value&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      data.boolean = rhs.data.boolean;
      break;
  case(ValueKind::integer):
      data.integer = rhs.data.integer;
      break;
  case(ValueKind::string):
      new(&data) std::string(std::move(rhs.data.string));
      break;
  case(ValueKind::function):
      data.function = rhs.data.function;
      break;
    }


  return *this;
}


Value
Value::
operator||(Value const&  rhs) const
{
  auto  a =     to_boolean();
  auto  b = rhs.to_boolean();

  return islog(a,b)? Value(a->boolean || b->boolean):Value(Undefined());
}


Value
Value::operator&&(Value const&  rhs) const
{
  auto  a =     to_boolean();
  auto  b = rhs.to_boolean();

  return islog(a,b)? Value(a->boolean && b->boolean):Value(Undefined());
}


Value
Value::
operator!() const
{
  auto  a = to_boolean();

  return (a == ValueKind::boolean)? Value(!a->boolean):Value(Undefined());
}




void
Value::
clear()
{
    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::undefined):
  case(ValueKind::boolean):
  case(ValueKind::integer):
  case(ValueKind::function):
      break;
  case(ValueKind::string):
      data.string.~basic_string();
      break;
    }


  kind = ValueKind::null;
}


Value
Value::
to_boolean() const
{
    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::function):
      break;
  case(ValueKind::undefined):
      return Value(false);
      break;
  case(ValueKind::boolean):
      return *this;
      break;
  case(ValueKind::integer):
      return Value(data.integer? true:false);
      break;
  case(ValueKind::string):
      return Value(data.string.size()? true:false);
      break;
    }


  return Value(Undefined());
}


void
Value::
print() const
{
    switch(kind)
    {
  case(ValueKind::null):
      break;
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      printf("%s",data.boolean? "true":"false");
      break;
  case(ValueKind::integer):
      printf("%d",data.integer);
      break;
  case(ValueKind::string):
      printf("\"%s\"",data.string.data());
      break;
  case(ValueKind::function):
      data.function->print();
      break;
    }
}




}




