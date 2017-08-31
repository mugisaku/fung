#ifndef FUNG_ValueKind_HPP_INCLUDED
#define FUNG_ValueKind_HPP_INCLUDED


#ifndef report
#define report  printf("[report at %s in %s,%8d]\n",__FILE__,__func__,__LINE__);
#endif


namespace fung{



enum class
ValueKind
{
  null,

  unevaluated,
  undefined,

  boolean,
  integer,
  function,
  string,
  list,

  any,

  nullpointer,
  pointer,
  reference,
  object,

};


class Object;
class Function;


}




#endif




