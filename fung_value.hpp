#ifndef FUNG_Value_HPP_INCLUDED
#define FUNG_Value_HPP_INCLUDED


#include"fung_ValueKind.hpp"
#include<string>
#include<vector>


namespace fung{



class Function;
class Value;


using ValueList = std::vector<Value>;


struct
Undefined
{
};


union
ValueData
{
  int         integer;
  bool        boolean;
  std::string  string;

  ValueList  list;

  Function const*  function;

   ValueData(){}
  ~ValueData(){}
};


class
Value
{
  ValueKind  kind=ValueKind::null;
  ValueData  data;

public:
   Value(){}
   explicit Value(Undefined  ud): kind(ValueKind::undefined){}
   explicit Value(int  i);
   explicit Value(bool  b);
   explicit Value(std::string&&  s);
   explicit Value(ValueList&&  ls);
   explicit Value(Function const*  fn);
   Value(Value const&  rhs) noexcept{*this = rhs;}
   Value(Value&&       rhs) noexcept{*this = std::move(rhs);}
  ~Value(){clear();}

  Value&  operator=(Value const&  rhs) noexcept;
  Value&  operator=(Value&&       rhs) noexcept;

  ValueData*        operator->()      {return &data;}
  ValueData const*  operator->() const{return &data;}

  bool  operator==(ValueKind   k) const{return kind == k;}
  bool  operator!=(ValueKind   k) const{return kind != k;}

  bool  is_undefined() const{return kind == ValueKind::undefined;}

  static bool  isarith(Value const&  a, Value const&  b){return((a == ValueKind::integer) &&
                                                                (b == ValueKind::integer));}

  static bool  islog(Value const&  a, Value const&  b){return((a == ValueKind::boolean) &&
                                                              (b == ValueKind::boolean));}

  int   i() const{return data.integer;}

  Value  operator+ (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()+rhs.i()):Value(Undefined());}
  Value  operator- (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()-rhs.i()):Value(Undefined());}
  Value  operator* (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()*rhs.i()):Value(Undefined());}
  Value  operator/ (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()/rhs.i()):Value(Undefined());}
  Value  operator% (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()%rhs.i()):Value(Undefined());}
  Value  operator<<(Value const&  rhs) const{return isarith(*this,rhs)? Value(i()<<rhs.i()):Value(Undefined());}
  Value  operator>>(Value const&  rhs) const{return isarith(*this,rhs)? Value(i()>>rhs.i()):Value(Undefined());}
  Value  operator| (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()|rhs.i()):Value(Undefined());}
  Value  operator& (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()&rhs.i()):Value(Undefined());}
  Value  operator^ (Value const&  rhs) const{return isarith(*this,rhs)? Value(i()^rhs.i()):Value(Undefined());}
  Value  operator==(Value const&  rhs) const{return isarith(*this,rhs)? Value(i() == rhs.i()):Value(Undefined());}
  Value  operator!=(Value const&  rhs) const{return isarith(*this,rhs)? Value(i() != rhs.i()):Value(Undefined());}
  Value  operator< (Value const&  rhs) const{return isarith(*this,rhs)? Value(i() <  rhs.i()):Value(Undefined());}
  Value  operator<=(Value const&  rhs) const{return isarith(*this,rhs)? Value(i() <= rhs.i()):Value(Undefined());}
  Value  operator> (Value const&  rhs) const{return isarith(*this,rhs)? Value(i() >  rhs.i()):Value(Undefined());}
  Value  operator>=(Value const&  rhs) const{return isarith(*this,rhs)? Value(i() >= rhs.i()):Value(Undefined());}

  Value  operator||(Value const&  rhs) const;
  Value  operator&&(Value const&  rhs) const;

  Value  operator!() const;
  Value  operator~() const{return(*this == ValueKind::integer)? Value(~i()):Value(Undefined());}
  Value  operator-() const{return(*this == ValueKind::integer)? Value(-i()):Value(Undefined());}

  operator bool() const{return kind != ValueKind::null;}

  void  clear();

  ValueKind  get_kind() const{return kind;}

  Value  to_boolean() const;

  void  print() const;

};




}




#endif




