#include"fung_branch.hpp"
#include<cstdio>
#include<string>
#include<ctime>


using namespace fung;


int
main(int  argc, char**  argv)
{
  auto  s = new Branch<char>("abc");

  {
    auto  b0 = s->bud();

    {
      auto  b1 = s->bud();

//delete s;
        if(b1.test())
        {
          b1.branch().print();
        }
    }


//delete s;
      if(b0.test())
      {
        b0.branch().print();
      }
  }


delete s;

  return 0;
}



