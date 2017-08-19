#include"fung_cursor.hpp"
#include"fung_unicode.hpp"


namespace fung{




std::string
Cursor::
null_path;




Cursor&
Cursor::
operator+=(int  n)
{
  pointer += n;

  return *this;
}


Cursor&
Cursor::
operator-=(int  n)
{
  pointer -= n;

  return *this;
}


void
Cursor::
newline()
{
         pointer += 1;
  head = pointer     ;

  line_count += 1;
}


int
Cursor::
get_column_point() const
{
  auto  p = head;

  int  n = 0;

    while(p < pointer)
    {
      p += utf8_byte_number(*p);

      n += 1;
    }


  return n;
}


bool
Cursor::
compare(const char*  s) const
{
  auto  const len = std::strlen(s);

  return(std::strncmp(pointer,s,len) == 0);
}




bool
Cursor::
compare(char  c0) const
{
  return(pointer[0] == c0);
}


bool
Cursor::
compare(char  c0, char  c1) const
{
  return((pointer[0] == c0) &&
         (pointer[1] == c1));
}


bool
Cursor::
compare(char  c0, char  c1, char  c2) const
{
  return((pointer[0] == c0) &&
         (pointer[1] == c1) &&
         (pointer[2] == c2));
}




void
Cursor::
print() const
{
  printf("File: %s\n",file_path? file_path->data():"?");

  printf("Line:%4d\n",line_count);

    if(pointer)
    {
      auto  p = head;

        while(*p && (*p != '\n'))
        {
            if(p == pointer)
            {
              printf("_$_");
            }


          printf("%c",*p++);
        }


      printf("\n");
    }
}




}




