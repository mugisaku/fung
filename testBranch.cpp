#include"fung_branch.hpp"
#include"fung_WeakReference.hpp"
#include<cstdio>
#include<string>
#include<ctime>


using namespace fung;


Branch<char>
process(Branch<char>  s, int  n)
{
  return n? process(s+'_',n-1):s;
}


std::string
process(std::string  s, int  n)
{
  return n? process(s+'_',n-1):s;
}


void
print(char const*  prefix, Branch<char> const&  s)
{
  printf("%s ID:%zd REF:%2d -- ",prefix,s.id(),s.use_count());

  s.print();

  printf("\n");
}


int
main(int  argc, char**  argv)
{
  Branch<char>  s0("abc");

  print("s0",s0);

  Branch<char>  s1(s0+"def");

  print("s1",s1);

  Branch<char>  s2 = process(s1,4);

  print("s2",s2);

  Branch<char>  s3(s1+"ghi");

  print("s3",s3);


  constexpr int  n = 8000;

  clock_t  start;

  start = std::clock();

  Branch<char>  s4 = process(s0,n);

  printf("s4 ID:%zd\n",s4.id());

  printf("%8d\n",std::clock()-start);


  start = std::clock();

  process(std::string(""),n);

  printf("%8d\n",std::clock()-start);

  return 0;
}



