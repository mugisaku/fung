#ifndef FUNG_HPP_INCLUDED
#define FUNG_HPP_INCLUDED


#include"fung_cursor.hpp"
#include"fung_token.hpp"
#include"fung_scope.hpp"
#include"fung_variable.hpp"
#include"fung_function.hpp"


namespace fung{


inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


std::unique_ptr<Scope>  make_global_scope(Cursor&  cur);


}




#endif




