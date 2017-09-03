#include"fung_value.hpp"
#include"fung_error.hpp"
#include"fung_function.hpp"




namespace fung{




namespace{
Value
add_p(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->pointer+v->integer):undefined;
}


Value
add_i(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer+v->integer):undefined;
}


Value
add_s(Value const&  lhs, Value const&  rhs)
{
  return (rhs == ValueKind::integer)? Value(lhs->string+rhs->integer):
         (rhs == ValueKind::string )? Value(lhs->string+rhs->string ):
           undefined;
}
}


Value
Value::
add(Value const&  lhs, Value const&  rhs)
{
  return (lhs == ValueKind::integer)? add_i(lhs,rhs):
         (lhs == ValueKind::pointer)? add_p(lhs,rhs):
         (lhs == ValueKind::string )? add_s(lhs,rhs):
         (lhs == ValueKind::list   )? Value(lhs->list+rhs):
           undefined;
}


Value
Value::
sub(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer-v->integer):undefined;
}


Value
Value::
mul(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer*v->integer):undefined;
}


Value
Value::
div(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer/v->integer):undefined;
}


Value
Value::
rem(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer%v->integer):undefined;
}


Value
Value::
shl(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer<<v->integer):undefined;
}


Value
Value::
shr(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer>>v->integer):undefined;
}


Value
Value::
bit_or(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer|v->integer):undefined;
}


Value
Value::
bit_and(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer&v->integer):undefined;
}


Value
Value::
bit_xor(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer^v->integer):undefined;
}


Value
Value::
eq(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer == v->integer):undefined;
}


Value
Value::
neq(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer != v->integer):undefined;
}


Value
Value::
lt(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer < v->integer):undefined;
}


Value
Value::
lteq(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer <= v->integer):undefined;
}


Value
Value::
gt(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer > v->integer):undefined;
}


Value
Value::
gteq(Value const&  lhs, Value const&  rhs)
{
  auto  v = rhs.convert_to_integer();

  return (v == ValueKind::integer)? Value(lhs->integer >= v->integer):undefined;
}






Value
Value::
log_not(Value const&  lhs)
{
  auto  a = lhs.convert_to_boolean();

  return (a == ValueKind::boolean)? Value(!a->boolean):undefined;
}


Value  Value::bit_not(Value const&  lhs){return(lhs == ValueKind::integer)? Value(~lhs->integer):undefined;}
Value  Value::    neg(Value const&  lhs){return(lhs == ValueKind::integer)? Value(-lhs->integer):undefined;}


template<typename  T>
Value
dereference(T&  t, size_t  i)
{
  return t.test()? Value(t.branch().at(i)):undefined;
}


Value
Value::
der(Value const&  lhs)
{
    if(lhs == ValueKind::pointer)
    {
      auto&  ptr = lhs->pointer;

           if(ptr == PointerKind::string){return dereference(ptr->string_bud,ptr.get_index());}
      else if(ptr == PointerKind::list  ){return dereference(ptr->list_bud  ,ptr.get_index());}
      else if(ptr == PointerKind::null  ){throw Error("NULLPTRを参照した");}
    }

  else
    {
      throw Error("ポインタでないものを参照した");
    }


  return undefined;
}




}




