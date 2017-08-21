#ifndef FUNG_Error_HPP_INCLUDED
#define FUNG_Error_HPP_INCLUDED


#include<cstring>
#include<cstdio>
#include<cstdarg>
#include<exception>
#include"fung_cursor.hpp"


#ifndef report
#define report  printf("[report at %s in %s,%8d]\n",__FILE__,__func__,__LINE__);
#endif

namespace fung{


struct
Error: public std::exception
{
  Cursor const  cursor;

  char  buffer[256];

  Error(char const*  fmt="", ...)
  {
    va_list  ap;
    va_start(ap,fmt);

    vsnprintf(buffer,sizeof(buffer),fmt,ap);

    va_end(ap);
  }

  Error(Cursor const&  cur, char const*  fmt="", ...): cursor(cur)
  {
    va_list  ap;
    va_start(ap,fmt);

    vsnprintf(buffer,sizeof(buffer),fmt,ap);

    va_end(ap);
  }

  char const*  what() noexcept{return buffer;}

};


}


#endif




