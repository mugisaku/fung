#ifndef FUNG_Cursor_HPP_INCLUDED
#define FUNG_Cursor_HPP_INCLUDED


#include<string>
#include<cstring>
#include<cstdio>


#ifndef report
#define report  printf("[report at %s in %s,%8d]\n",__FILE__,__func__,__LINE__);
#endif

namespace fung{


class
Cursor
{
  static std::string  null_path;

  std::string const*  file_path=&null_path;

  char const*     head=nullptr;
  char const*  pointer=nullptr;

  int  line_count=1;

public:
  constexpr Cursor(){}

  explicit Cursor(char const*  p, std::string const*  file_path_=nullptr):
  file_path(file_path_? file_path_:&null_path),
  head(p), pointer(p){}

  Cursor&  operator++(){++pointer;  return *this;}
  Cursor&  operator--(){--pointer;  return *this;}
  Cursor&  operator+=(int  n);
  Cursor&  operator-=(int  n);

  char const&  operator[](int  i) const{return pointer[i];}
  char const&   operator*() const{return *pointer;}

  operator bool() const{return pointer && *pointer;}

  int  get_line_count() const{return line_count;}
  int  get_column_point() const;

  std::string const&  get_file_path() const{return *file_path;}

  char const*  to_pointer() const{return pointer;};

  void  newline();

  bool  compare(const char*  s) const;

  bool  compare(char  c0                    ) const;
  bool  compare(char  c0, char  c1          ) const;
  bool  compare(char  c0, char  c1, char  c2) const;

  void  print() const;

};


void  skip_space(             Cursor&  cur);
void  skip_spaces(            Cursor&  cur);
void  skip_spaces_and_newline(Cursor&  cur);

void  skip_blockstyle_comment(Cursor&  cur);
void  skip_linestyle_comment( Cursor&  cur);


}


#endif




