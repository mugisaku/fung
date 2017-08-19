#ifndef FUNG_ValueKind_HPP_INCLUDED
#define FUNG_ValueKind_HPP_INCLUDED


namespace fung{



enum class
ValueKind
{
  null,

  undefined,
  boolean,
  integer,
  function,
  string,
  list,

  nullpointer,
  pointer,
  reference,
  object,

};


class Object;
class Function;


}




#endif




