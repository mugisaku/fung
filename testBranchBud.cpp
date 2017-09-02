#include"fung_branch.hpp"
#include<cstdio>
#include<string>
#include<ctime>


using namespace fung;


template<typename  T>
void
remove(T&  ptr)
{
  delete ptr          ;
         ptr = nullptr;
}


int
main(int  argc, char**  argv)
{
  auto  b  = new Branch<char>("abc");
  auto  bx = new Branch<char>(*b);

  {
    auto  b0 = b->bud();

    {
      auto  b1 = b->bud();

remove(b);
remove(bx);
    }


//remove(b);
  }


remove(b);
remove(bx);

  return 0;
}



