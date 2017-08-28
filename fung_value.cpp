#include"fung_value.hpp"
#include"fung_function.hpp"




namespace fung{




Value  const undefined(ValueKind::undefined);




Value::Value(int  i): kind(ValueKind::integer){data.integer = i;}
Value::Value(bool  b): kind(ValueKind::boolean){data.boolean = b;}
Value::Value(Function const*  fn): kind(ValueKind::function){data.function = fn;}
Value::Value(std::string&&  s): kind(ValueKind::string){new(&data) SharedString<char>(std::move(s));}
Value::Value(SharedString<char>&&  s): kind(ValueKind::string){new(&data) SharedString<char>(std::move(s));}
Value::Value(Value*  v): kind(ValueKind::any){data.value = v;}
Value::Value(ValueList&&  ls): kind(ValueKind::list){new(&data) ValueList(std::move(ls));}




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
      new(&data) SharedString<char>(rhs.data.string);
      break;
  case(ValueKind::any):
      data.value = new Value(*rhs.data.value);
      break;
  case(ValueKind::list):
      new(&data) ValueList(rhs.data.list);
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
      new(&data) SharedString<char>(std::move(rhs.data.string));
      break;
  case(ValueKind::any):
      data.value = rhs.data.value;
      break;
  case(ValueKind::list):
      new(&data) ValueList(std::move(rhs.data.list));
      break;
  case(ValueKind::function):
      data.function = rhs.data.function;
      break;
    }


  return *this;
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
      data.string.~SharedString();
      break;
  case(ValueKind::any):
      delete data.value;
      break;
  case(ValueKind::list):
      data.list.~vector();
      break;
    }


  kind = ValueKind::null;
}


void
Value::
print() const
{
    switch(kind)
    {
  case(ValueKind::null):
      printf("NULL ");
      break;
  case(ValueKind::undefined):
      printf("UNDEFINED ");
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
  case(ValueKind::any):
      data.value->print();
      break;
  case(ValueKind::list):
      {
        auto   it = data.list.cbegin();
        auto  end = data.list.cend();

          if(it != end)
          {
            it++->print();

              while(it != end)
              {
                printf(",");

                it++->print();
              }
          }
      }
      break;
    }
}




ValueKind
Value::
to_kind(std::string const&  s)
{
       if(s == "integer" ){return ValueKind::integer;}
  else if(s == "boolean" ){return ValueKind::boolean;}
  else if(s == "string"  ){return ValueKind::string;}
  else if(s == "function"){return ValueKind::function;}
  else if(s == "list"    ){return ValueKind::list;}
  else if(s == "any"     ){return ValueKind::any;}


  return ValueKind::undefined;
}


std::string const&
Value::
to_string(ValueKind  k)
{
  static std::string  const i("integer");
  static std::string  const s("string");
  static std::string  const b("boolean");
  static std::string  const l("list");
  static std::string  const a("any");
  static std::string  const u("undefined");
  static std::string  const f("function");

    switch(k)
    {
  case(ValueKind::null):
      break;
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      return b;
      break;
  case(ValueKind::integer):
      return i;
      break;
  case(ValueKind::function):
      return f;
      break;
  case(ValueKind::string):
      return s;
      break;
  case(ValueKind::list):
      return l;
      break;
  case(ValueKind::any):
      return a;
      break;
    }


  return u;
}


}




