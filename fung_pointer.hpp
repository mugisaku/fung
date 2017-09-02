#ifndef FUNG_Pointer_HPP_INCLUDED
#define FUNG_Pointer_HPP_INCLUDED


#include"fung_ValueKind.hpp"
#include"fung_branch.hpp"


namespace fung{



class Value;

using   List = Branch<Value>;
using String = Branch<char> ;


enum class
PointerKind
{
  null,
  string,
  list,

};


union
PointerData
{
  String::Bud  string_bud;
  List::Bud      list_bud;

  PointerData(){}
 ~PointerData(){}

};


class
Pointer
{
  PointerKind  kind=PointerKind::null;
  PointerData  data;

  size_t  index=0;

public:
  Pointer(){}
  Pointer(String const&  s);
  Pointer(List const&  ls);
  Pointer(Pointer const&  rhs) noexcept{*this = rhs;}
  Pointer(Pointer&&  rhs) noexcept{*this = std::move(rhs);}
 ~Pointer(){clear();}

  Pointer&  operator=(Pointer const&  rhs) noexcept;
  Pointer&  operator=(Pointer&&  rhs) noexcept;

  PointerData const*  operator->() const{return &data;}  

  bool  operator==(PointerKind  k) const{return kind == k;}

  void  clear();

  PointerKind  get_kind() const{return kind;}

  void    set_index(size_t  i){index = i;}
  size_t  get_index() const{return index;}

  void  print() const;

};


}




#endif




