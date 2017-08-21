#ifndef FUNG_HPP_INCLUDED
#define FUNG_HPP_INCLUDED


#include"fung_error.hpp"
#include"fung_token.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"


namespace fung{


inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


std::unique_ptr<GlobalSpace>  make_global_space(Cursor&  cur);


}




#endif




