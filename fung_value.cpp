#include"fung_value.hpp"
#include"fung_function.hpp"




namespace fung{




Value  const undefined(ValueKind::undefined);




Value::Value(int  i): kind(ValueKind::integer){data.integer = i;}
Value::Value(bool  b): kind(ValueKind::boolean){data.boolean = b;}
Value::Value(Function const&  fn): kind(ValueKind::function){data.function = &fn;}
Value::Value(std::string const&  s): kind(ValueKind::string){new(&data) String(s.data(),s.size());}
Value::Value(String&&  s): kind(ValueKind::string){new(&data) String(std::move(s));}
Value::Value(Value*  v): kind(ValueKind::any){data.value = v;}
Value::Value(List&&  ls): kind(ValueKind::list){new(&data) List(std::move(ls));}
Value::Value(Pointer&&  ptr): kind(ValueKind::pointer){new(&data) Pointer(std::move(ptr));}




Value&
Value::
operator=(Value const&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

    switch(kind)
    {
  case(ValueKind::null):
  case(ValueKind::unevaluated):
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      data.boolean = rhs.data.boolean;
      break;
  case(ValueKind::integer):
      data.integer = rhs.data.integer;
      break;
  case(ValueKind::string):
      new(&data) String(rhs.data.string);
      break;
  case(ValueKind::any):
      data.value = new Value(*rhs.data.value);
      break;
  case(ValueKind::list):
      new(&data) List(rhs.data.list);
      break;
  case(ValueKind::pointer):
      new(&data) Pointer(rhs.data.pointer);
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
  case(ValueKind::unevaluated):
  case(ValueKind::undefined):
      break;
  case(ValueKind::boolean):
      data.boolean = rhs.data.boolean;
      break;
  case(ValueKind::integer):
      data.integer = rhs.data.integer;
      break;
  case(ValueKind::string):
      new(&data) String(std::move(rhs.data.string));
      break;
  case(ValueKind::any):
      data.value = rhs.data.value;
      break;
  case(ValueKind::list):
      new(&data) List(std::move(rhs.data.list));
      break;
  case(ValueKind::pointer):
      new(&data) Pointer(std::move(rhs.data.pointer));
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
  case(ValueKind::unevaluated):
  case(ValueKind::undefined):
  case(ValueKind::boolean):
  case(ValueKind::integer):
  case(ValueKind::function):
      break;
  case(ValueKind::string):
      data.string.~Branch();
      break;
  case(ValueKind::any):
      delete data.value;
      break;
  case(ValueKind::list):
      data.list.~Branch();
      break;
  case(ValueKind::pointer):
      data.pointer.~Pointer();
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
  case(ValueKind::unevaluated):
      printf("UNEVALUATED ");
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
      data.string.print();
      break;
  case(ValueKind::function):
      data.function->print();
      break;
  case(ValueKind::any):
      data.value->print();
      break;
  case(ValueKind::pointer):
      data.pointer.print();
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
  else if(s == "pointer" ){return ValueKind::pointer;}
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
  static std::string  const n("unevaluated");
  static std::string  const f("function");
  static std::string  const p("pointer");

    switch(k)
    {
  case(ValueKind::null):
      break;
  case(ValueKind::unevaluated):
      return n;
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
  case(ValueKind::pointer):
      return p;
      break;
  case(ValueKind::any):
      return a;
      break;
    }


  return u;
}


}




