#include"fung_value.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{




Value
Value::
get_property(std::string const&  name) const
{
    switch(kind)
    {
  case(ValueKind::null):
      break;
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      break;
  case(ValueKind::integer):
      break;
  case(ValueKind::function):
      break;
  case(ValueKind::string):
           if(name ==  "length"){return Value(static_cast<int>(data.string.length()));}
      else if(name == "pointer"){return Value(Pointer(data.string));}
      break;
  case(ValueKind::list):
           if(name == "length"){return Value(static_cast<int>(data.list.length()));}
      else if(name == "pointer"){return Value(Pointer(data.list));}
      break;
  case(ValueKind::any):
      break;
    }


  throw Error("%sというプロパティーは存在しない",name.data());
}




}



