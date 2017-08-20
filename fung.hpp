#ifndef FUNG_HPP_INCLUDED
#define FUNG_HPP_INCLUDED


#include"fung_cursor.hpp"
#include"fung_token.hpp"
#include"fung_space.hpp"
#include"fung_variable.hpp"
#include"fung_function.hpp"


namespace fung{


inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


std::unique_ptr<Space>  make_global_space(Cursor&  cur);


}




#endif




