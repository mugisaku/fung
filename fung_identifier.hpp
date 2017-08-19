#ifndef FUNG_Identifier_HPP_INCLUDED
#define FUNG_Identifier_HPP_INCLUDED


#include<string>
#include<cctype>


namespace fung{


class Cursor;


struct
Identifier
{
  std::string  string;

  Identifier(std::string&&  s): string(std::move(s)){}

  std::string*        operator->()      {return &string;}
  std::string const*  operator->() const{return &string;}

};


constexpr bool  isident0(char  c){return(isalpha(c) || (c == '_'));}
constexpr bool  isidentn(char  c){return(isalnum(c) || (c == '_'));}


std::string  read_identifier(Cursor&  cur);


}




#endif




