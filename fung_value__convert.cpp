#include"fung_value.hpp"
#include"fung_function.hpp"
#include<cstdlib>




namespace fung{




Value
Value::
convert_to_boolean() const
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
  case(ValueKind::character):
      return Value(data.character? true:false);
      break;
  case(ValueKind::string):
      return Value(data.string.length()? true:false);
      break;
  case(ValueKind::list):
      return Value(data.list.length()? true:false);
      break;
    }


  return undefined;
}


Value
Value::
convert_to_string() const
{
    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::function):
  case(ValueKind::undefined):
  case(ValueKind::list):
      break;
  case(ValueKind::boolean):
      return Value(data.boolean? "true":"false");
      break;
  case(ValueKind::integer):
      return Value(std::to_string(data.integer));
      break;
  case(ValueKind::string):
      return *this;
      break;
    }


  return undefined;
}


Value
Value::
convert_to_integer() const
{
    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::function):
  case(ValueKind::undefined):
  case(ValueKind::boolean):
  case(ValueKind::list):
      break;
  case(ValueKind::integer):
      return *this;
      break;
  case(ValueKind::character):
      return Value(static_cast<int>(data.character));
      break;
  case(ValueKind::string):
      return Value(static_cast<int>(std::strtol(data.string.data(),nullptr,0)));
      break;
    }


  return undefined;
}




}




