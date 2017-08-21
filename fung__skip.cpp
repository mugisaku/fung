#include"fung_cursor.hpp"
#include"fung_error.hpp"


namespace fung{




void
skip_space(Cursor&  cur)
{
    while(*cur == ' ')
    {
      cur += 1;
    }
}




void
skip_spaces(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if((c == ' ') || (c == '\t') || (c == '\r'))
        {
          cur += 1;
        }

      else
        if(cur.compare('\\','\n'))
        {
          cur += 1;

          cur.newline();
        }

      else
        {
          break;
        }
    }
}


void
skip_spaces_and_newline(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if((c == ' ') || (c == '\t') || (c == '\r'))
        {
          cur += 1;
        }

      else
        if(c == '\n')
        {
          cur.newline();
        }

      else
        if(cur.compare('\\','\n'))
        {
          cur += 1;

          cur.newline();
        }

      else
        {
          break;
        }
    }
}




void
skip_blockstyle_comment(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if(cur.compare('*','/'))
        {
          cur += 2;

          break;
        }

      else
        if(c == '\n')
        {
          cur.newline();
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"ブロックスタイルコメントの途中で終端文字");
        }

      else
        {
          cur += 1;
        }
    }
}


void
skip_linestyle_comment(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if(c == '\n')
        {
          break;
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"ラインスタイルコメントの途中で終端文字");
        }


      cur += 1;
    }
}




}




