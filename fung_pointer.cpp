#include"fung_pointer.hpp"




namespace fung{




Pointer::Pointer(String const&  s): kind(PointerKind::string){new(&data) String::Bud(s.bud());}
Pointer::Pointer(List const&  ls): kind(PointerKind::list){new(&data) List::Bud(ls.bud());}




Pointer&
Pointer::
operator=(Pointer const&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

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




