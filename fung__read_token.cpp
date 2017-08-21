#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"fung_token.hpp"
#include"fung_error.hpp"


namespace fung{


namespace{


unsigned int
read_binary_number_literal(Cursor&  cur)
{
  cur += 1;


  unsigned int  n = 0;

    for(;;)
    {
      auto  const c = *cur;

        if(!isbinary(c))
        {
          break;
        }


      cur += 1;

      n <<= 1;
      n  |= c-'0';
    }


  return n;
}


unsigned int
read_octal_number_literal(Cursor&  cur)
{
  cur += 1;


  unsigned int  n = 0;

    for(;;)
    {
      auto  const c = *cur;

        if(!isoctal(c))
        {
          break;
        }


      cur += 1;

      n <<= 3;
      n  |= c-'0';
    }


  return n;
}


unsigned int
read_decimal_number_literal(Cursor&  cur)
{
  cur += 1;


  unsigned int  n = 0;

    for(;;)
    {
      auto  const c = *cur;

        if(!isdecimal(c))
        {
          break;
        }


      cur += 1;

      n *=    10;
      n += c-'0';
    }


  return n;
}


unsigned int
read_hexadecimal_number_literal(Cursor&  cur)
{
  cur += 1;


  unsigned int  n = 0;

    for(;;)
    {
      auto  const c = *cur;

        if(isdecimal(c))
        {
          cur += 1;

          n <<= 4;
          n  |= c-'0';
        }

      else
        if(((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F')))
        {
          cur += 1;

          n <<= 4;

            switch(c)
            {
          case('a'):
          case('A'): n |= 10;break;
          case('b'):
          case('B'): n |= 11;break;
          case('c'):
          case('C'): n |= 12;break;
          case('d'):
          case('D'): n |= 13;break;
          case('e'):
          case('E'): n |= 14;break;
          case('f'):
          case('F'): n |= 15;break;
            }
        }

      else
        {
          break;
        }
    }


  return n;
}


bool
op(TinyString&  ts, char  c0, char  c1=0, char  c2=0)
{
  ts = TinyString(c0,c1,c2);

  return true;
}


bool
check(Cursor&  cur, TinyString&  ts, char  c0)
{
  return cur.compare(c0)? op(ts,c0):false;
}


bool
check(Cursor&  cur, TinyString&  ts, char  c0, char  c1)
{
  return cur.compare(c0,c1)? op(ts,c0,c1):false;
}


bool
check(Cursor&  cur, TinyString&  ts, char  c0, char  c1, char  c2)
{
  return cur.compare(c0,c1,c2)? op(ts,c0,c1,c2):false;
}


}


TinyString
read_operator(Cursor&  cur)
{
  TinyString  ts;

       if(check(cur,ts,'.','.','.')){}
  else if(check(cur,ts,'<','<','=')){}
  else if(check(cur,ts,'>','>','=')){}
  else if(check(cur,ts,'<','<'    )){}
  else if(check(cur,ts,'<','='    )){}
  else if(check(cur,ts,'<'        )){}
  else if(check(cur,ts,'>','>'    )){}
  else if(check(cur,ts,'>','='    )){}
  else if(check(cur,ts,'>'        )){}
  else if(check(cur,ts,'+','+'    )){}
  else if(check(cur,ts,'-','-'    )){}
  else if(check(cur,ts,'-','>'    )){}
  else if(check(cur,ts,'+','='    )){}
  else if(check(cur,ts,'-','='    )){}
  else if(check(cur,ts,'*','='    )){}
  else if(check(cur,ts,'/','='    )){}
  else if(check(cur,ts,'%','='    )){}
  else if(check(cur,ts,'|','='    )){}
  else if(check(cur,ts,'&','='    )){}
  else if(check(cur,ts,'^','='    )){}
  else if(check(cur,ts,'=','='    )){}
  else if(check(cur,ts,'!','='    )){}
  else if(check(cur,ts,'|','|'    )){}
  else if(check(cur,ts,'&','&'    )){}
  else if(check(cur,ts,':',':'    )){}
  else if(check(cur,ts,':'        )){}
  else if(check(cur,ts,';'        )){}
  else if(check(cur,ts,'='        )){}
  else if(check(cur,ts,'!'        )){}
  else if(check(cur,ts,'?'        )){}
  else if(check(cur,ts,'#'        )){}
  else if(check(cur,ts,'~'        )){}
  else if(check(cur,ts,'^'        )){}
  else if(check(cur,ts,'+'        )){}
  else if(check(cur,ts,'-'        )){}
  else if(check(cur,ts,'*'        )){}
  else if(check(cur,ts,'/'        )){}
  else if(check(cur,ts,'%'        )){}
  else if(check(cur,ts,'|'        )){}
  else if(check(cur,ts,'&'        )){}
  else if(check(cur,ts,','        )){}
  else if(check(cur,ts,'.'        )){}
  else if(check(cur,ts,'('        )){}
  else if(check(cur,ts,')'        )){}
  else if(check(cur,ts,'['        )){}
  else if(check(cur,ts,']'        )){}
  else if(check(cur,ts,'{'        )){}
  else if(check(cur,ts,'}'        )){}
  else{throw Error(cur,"不明な演算子");}


  cur += ts.length();

  return ts;
}


std::string
read_identifier(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  c = *cur;

        if(isidentn(c))
        {
          cur += 1;

          s.push_back(c);
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::string
read_string_literal(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(c == '\\')
        {
          s.push_back('\\');

          cur += 1;

          s.push_back(*cur);

          cur += 1;
        }

      else
        if(c == '\"')
        {
          cur += 1;

          break;
        }

      else
        if(iscntrl(c))
        {
          throw Error(cur,"クオート列に制御文字 %d",c);
        }

      else
        {
          cur += 1;

          s.push_back(c);
        }
    }


  return std::move(s);
}


unsigned int
read_character_literal(Cursor&  cur)
{
    if(*cur == '\\')
    {
/*
          switch(*++p)
          {
        case('0'):  return '\0';
        case('t'):  return '\t';
        case('r'):  return '\r';
        case('n'):  return '\n';
        case('\\'): return '\\';
        case('\''): return '\'';
        case('\"'): return '\"';
        case('x'): return read_hexadecimal(p+1);
        default: throw Error("処理不可なエスケープ文字 %c",*p);
          }
*/
    }


  return 0;
}




Token
read_token(Cursor&  cur)
{
  skip_spaces_and_newline(cur);


  Token  tok;

  TokenInfo  info(cur);

  auto  const c = *cur;

    if(!c)
    {
    }

  else
    if(c == '\'')
    {
      cur += 1;

      tok = Token(read_character_literal(cur),std::move(info));
    }

  else
    if(c == '\"')
    {
      cur += 1;

      tok = Token(read_string_literal(cur),std::move(info));
    }

  else
    if(cur.compare('/','*'))
    {
      cur += 2;

      skip_blockstyle_comment(cur);
    }

  else
    if(cur.compare('/','/'))
    {
      cur += 2;

      skip_linestyle_comment(cur);
    }

  else
    if(c == '0')
    {
      cur += 1;

      auto  const cc = *cur;

      tok = ((cc == 'b')? Token(     read_binary_number_literal(cur),std::move(info)):
             (cc == 'B')? Token(     read_binary_number_literal(cur),std::move(info)):
             (cc == 'o')? Token(      read_octal_number_literal(cur),std::move(info)):
             (cc == 'O')? Token(      read_octal_number_literal(cur),std::move(info)):
             (cc == 'x')? Token(read_hexadecimal_number_literal(cur),std::move(info)):
             (cc == 'X')? Token(read_hexadecimal_number_literal(cur),std::move(info)):
                          Token(                                  0U,std::move(info)));
    }

  else
    if(isdigit(c))
    {
      cur -= 1;

      tok = Token(read_decimal_number_literal(cur),std::move(info));
    }

  else
    if(isident0(c))
    {
      tok = Token(Identifier(read_identifier(cur)),std::move(info));
    }

  else
    if(ispunct(c))
    {
      tok = Token(read_operator(cur),std::move(info));
    }

  else
    {
      throw Error(cur,"処理不可の文字 %c(%d)",c,c);
    }


  return std::move(tok);
}




}




