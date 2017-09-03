#include"fung_pointer.hpp"
#include"fung_value.hpp"
#include"fung_error.hpp"




namespace fung{




Pointer::Pointer(String const&  s, size_t  i): kind(PointerKind::string), index(i){new(&data) String::Bud(s.bud());}
Pointer::Pointer(List const&   ls, size_t  i): kind(PointerKind::list  ), index(i){new(&data) List::Bud(ls.bud());}




Pointer&
Pointer::
operator=(Pointer const&  rhs) noexcept
{
  clear();

  kind  = rhs.kind;
  index = rhs.index;

    switch(kind)
    {
  case(PointerKind::null):
      break;
  case(PointerKind::string):
      new(&data) String::Bud(rhs.data.string_bud);
      break;
  case(PointerKind::list):
      new(&data) List::Bud(rhs.data.list_bud);
      break;
    }


  return *this;
}


Pointer&
Pointer::
operator=(Pointer&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

  index = rhs.index;

    switch(kind)
    {
  case(PointerKind::null):
      break;
  case(PointerKind::string):
      new(&data) String::Bud(std::move(rhs.data.string_bud));
      break;
  case(PointerKind::list):
      new(&data) List::Bud(std::move(rhs.data.list_bud));
      break;
    }


  return *this;
}




void
Pointer::
clear()
{
    switch(kind)
    {
  case(PointerKind::null):
      break;
  case(PointerKind::string):
      data.string_bud.~Bud();
      break;
  case(PointerKind::list):
      data.list_bud.~Bud();
      break;
    }


  kind = PointerKind::null;
}


Pointer
Pointer::
operator+(size_t  i) const
{
    switch(kind)
    {
  case(PointerKind::null):
      return Pointer();
      break;
  case(PointerKind::string):
      return Pointer(data.string_bud.branch(),index+i);
      break;
  case(PointerKind::list):
      return Pointer(data.list_bud.branch(),index+i);
      break;
    }


throw Error("不明なポインタ");
  return Pointer();
}


void
Pointer::
print() const
{
    switch(kind)
    {
  case(PointerKind::null):
      printf("NULLPTR ");
      break;
  case(PointerKind::string):
      printf("STRINGPTR(%zd)",index);
      break;
  case(PointerKind::list):
      printf("LISTPTR(%zd)",index);
      break;
    }
}




}




