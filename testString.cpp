#include"fung_SharedString.hpp"
#include<cstdio>
#include<string>
#include<ctime>


using namespace fung;


SharedString<char>
process(SharedString<char>  s, int  n)
{
  return n? process(s+'_',n-1):s;
}


std::string
process(std::string  s, int  n)
{
  return n? process(s+'_',n-1):s;
}


void
print(char const*  prefix, SharedString<char> const&  s)
{
  printf("%s ID:%zd REF:%2d \"",prefix,s.id(),s.use_count());

  s.print();

  printf("\"\n");
}


int
main(int  argc, char**  argv)
{
  SharedString<char>  s0("abc");

  print("s0",s0);

  SharedString<char>  s1(s0+"def");

  print("s1",s1);

  SharedString<char>  s2 = process(s1,4);

  print("s2",s2);

  SharedString<char>  s3(s1+"ghi");

  print("s3",s3);


  constexpr int  n = 8000;

  clock_t  start;

  start = std::clock();

  SharedString<char>  s4 = process(s0,n);

  printf("s4 ID:%zd\n",s4.id());

  printf("%8d\n",std::clock()-start);


  start = std::clock();

  process(std::string(""),n);

  printf("%8d\n",std::clock()-start);

  return 0;
}



