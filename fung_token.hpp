#ifndef FUNG_Token_HPP_INCLUDED
#define FUNG_Token_HPP_INCLUDED


#include<string>
#include<memory>
#include<vector>
#include"fung_cursor.hpp"
#include"fung_identifier.hpp"
#include"fung_TinyString.hpp"


namespace fung{


constexpr bool  isbinary( char  c){return (c == '0') || (c == '1');}
constexpr bool  isoctal(  char  c){return (c >= '0') && (c <= '7');}
constexpr bool  isdecimal(char  c){return (c >= '0') && (c <= '9');}


enum class
TokenKind
{
  null,

  integer,
  string,
  identifier,
  operator_,

};


class
TokenInfo
{
  std::string const*  file_path;

  int    line_count=0;
  int  column_point=0;

public:
  TokenInfo(){}
  TokenInfo(Cursor&  cur):
  file_path(&cur.get_file_path()),
  line_count(cur.get_line_count()),
  column_point(cur.get_column_point()){}

  std::string const&  get_file_path() const{return *file_path;}

  int  get_line_count() const{return line_count;}
  int  get_column_point() const{return column_point;}

};


union
TokenData
{
  unsigned int     integer;
  std::string       string;
  TinyString   tiny_string;

   TokenData(){}
  ~TokenData(){}
};


class
Token
{
  TokenKind  kind=TokenKind::null;

  TokenData  data;
  TokenInfo  info;

public:
  Token(){}
  Token(unsigned int       i, TokenInfo&&  info_=TokenInfo());
  Token(std::string&&      s, TokenInfo&&  info_=TokenInfo());
  Token(Identifier&&      id, TokenInfo&&  info_=TokenInfo());
  Token(TinyString const&  o, TokenInfo&&  info_=TokenInfo());
  Token(Token&&  rhs) noexcept{*this = std::move(rhs);}
  Token(Token const&  rhs) = delete;
 ~Token(){clear();}


  Token&  operator=(Token&&  rhs) noexcept;
  Token&  operator=(Token const&  rhs) =delete;

  bool  operator==(TokenKind  k) const{return kind == k;}
  bool  operator!=(TokenKind  k) const{return kind != k;}

  bool  operator==(TinyString const&  ts) const{return *data.tiny_string == *ts;}

  TokenData      *  operator->()      {return &data;}
  TokenData const*  operator->() const{return &data;}

  operator bool() const{return kind != TokenKind::null;}

  void  clear();

  TokenKind  get_kind() const{return kind;}

  void  set_info(TokenInfo&&  info_){info = std::move(info_);}
  TokenInfo const&  get_info() const{return info;}

  void  print(FILE*  out=stdout) const;

};


Token  read_token(Cursor&  cur);


}




#endif




