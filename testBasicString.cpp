#include"fung_BasicString.hpp"
#include<cstdio>
#include<string>
#include<ctime>


using namespace fung;


void
print(BasicString<char> const&  s)
{
  printf("%s\n",s.data());
}


int
main(int  argc, char**  argv)
{
  BasicString<char>  s0("abc_");
  BasicString<char>  s1("def_");
  BasicString<char>  s2("ghi");


  print(s0);
  print(s1);
  print(s2);

  print(s0+s1+s2+"jklmn");
  

  return 0;
}



