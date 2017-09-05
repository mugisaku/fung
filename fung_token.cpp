#include"fung_token.hpp"
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<new>




namespace fung{


Token::Token(unsigned int       i, TokenInfo&&  info_): kind(TokenKind::integer   ), info(std::move(info_)){data.integer = i;}
Token::Token(std::string&&      s, TokenInfo&&  info_): kind(TokenKind::string    ), info(std::move(info_)){new(&data) std::string(std::move(s));}
Token::Token(Identifier&&      id, TokenInfo&&  info_): kind(TokenKind::identifier), info(std::move(info_)){new(&data) std::string(std::move(id.string));}
Token::Token(TinyString const&  o, TokenInfo&&  info_): kind(TokenKind::operator_ ), info(std::move(info_)){data.tiny_string = o;}


Token&
Token::
operator=(Token&&  rhs) noexcept
{
    if(&rhs != this)
    {
      clear();

      std::swap(kind,rhs.kind);

        switch(kind)
        {
      case(TokenKind::null):
          break;
      case(TokenKind::integer):
          data.integer = rhs.data.integer;
          break;
      case(TokenKind::identifier):
      case(TokenKind::string):
          new(&data) std::string(std::move(rhs.data.string));
          break;
      case(TokenKind::operator_):
          data.tiny_string = rhs.data.tiny_string;
          break;
        }
    }


  return *this;
}




void
Token::
clear()
{
    switch(kind)
    {
  case(TokenKind::null):
  case(TokenKind::integer):
  case(TokenKind::operator_):
      break;
  case(TokenKind::identifier):
  case(TokenKind::string):
      data.string.~basic_string();
      break;
    }


  kind = TokenKind::null;
}


void
Token::
print(FILE*  out) const
{
    switch(kind)
    {
  case(TokenKind::null):
      fprintf(out,"NULL");
      break;
  case(TokenKind::integer):
      fprintf(out,"%ud",data.integer);
      break;
  case(TokenKind::identifier):
      fprintf(out,"%s",data.string.data());
      break;
  case(TokenKind::string):
      fprintf(out,"\"%s\"",data.string.data());
      break;
  case(TokenKind::operator_):
      fprintf(out,"%s",data.tiny_string.codes);
      break;
    }
}



}




