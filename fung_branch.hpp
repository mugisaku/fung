#ifndef FUNG_Branch_HPP_INCLUDED
#define FUNG_Branch_HPP_INCLUDED


#include"fung_BasicString.hpp"
#include<cstdio>
#include<cstdint>
#include<initializer_list>


namespace fung{



template<typename  T>
class
Branch
{
  class String: public BasicString<T>
  {
    struct{
      size_t  reference_count=0;

    } var;

  public:
    template<typename...  Args>
    String(Args...  args): BasicString<T>(args...){}

    void      refer(){       ++var.reference_count;}
    size_t  unrefer(){return --var.reference_count;}

    size_t  reference_count() const{return var.reference_count;}

    void  start_reference_count(){var.reference_count = 1;}
  };

public:
  class Link{
    struct{
      String*  target=nullptr;

      size_t  reference_count=0;

    } var;

  public:
    Link(String*  target_){var.target = target_;}

    String*  target() const{return var.target;}

    void      refer(){       ++var.reference_count;}
    size_t  unrefer(){return --var.reference_count;}

    void  unlink(){var.target = nullptr;}

    size_t  reference_count() const{return var.reference_count;}

  };


  class Bud{
    struct{
      Link*  link=nullptr;

      size_t  length=0;

    } var;

  public:
    Bud(Link&  ln, size_t  length)
    {
      var.link   =    &ln;
      var.length = length;

      ln.refer();
    }

   ~Bud()
    {
        if(var.link)
        {
          auto  s = var.link->target();

            if(!var.link->unrefer())
            {
                if(!s)
                {
                  delete var.link;
printf("Link was deleted by Bud\n");
                }
            }
        }
    }


    bool  test() const{return var.link->target();}

    Branch  branch() const{return Branch(var.link->target(),var.length);}

  };

private:
  struct{
    String*  string=nullptr;

    size_t  length=0;

    Link*  link=nullptr;

  } var;


public:
  Branch()
  {
    var.string = new String();
    var.length = 0;

    var.string->start_reference_count();
  }

  Branch(std::initializer_list<T>  ls)
  {
    var.string = new String(ls);
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  Branch(T const*  src)
  {
    var.string = new String(src);
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  Branch(T const*  src, size_t  len)
  {
    var.string = new String(src,len);
    var.length = len;

    var.string->start_reference_count();
  }

  Branch(String&&  s)
  {
    var.string = new String(std::move(s));
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  Branch(String*  s, size_t  len=0)
  {
    var.string = s;
    var.length = len;

      if(s)
      {
        s->refer();
      }
  }

  Branch(Branch const&  rhs) noexcept
  {
    var.string = rhs.var.string;
    var.length = 0;

      if(var.string)
      {
        var.length = var.string->length();

        var.string->refer();
      }
  }

  Branch(Branch&&  rhs) noexcept
  {
    var.string = rhs.var.string          ;
                 rhs.var.string = nullptr;

    var.length = rhs.var.length    ;
                 rhs.var.length = 0;

    var.link = rhs.var.link          ;
               rhs.var.link = nullptr;
  }
 
 ~Branch(){clear();}

  Branch&  operator=(Branch const&  rhs) noexcept=delete;
  Branch&  operator=(Branch&&       rhs) noexcept=delete;


  Branch  operator+(Branch const&  rhs) const{return add(rhs);}
  Branch  operator+(T const*  src)            const{return add(src);}
  Branch  operator+(T const&    t)            const{return add(t);}


  String const&  operator*() const{return *var.string;}
  String const*  operator->() const{return var.string;}


  Branch  add(Branch const&  rhs) const{return add(rhs.data(),rhs.length());}
  Branch  add(T const*  src)            const{return add(src,String::length(src));}

  Branch  add(T const*  src, size_t  len) const
  {
      if(length() == var.string->length())
      {
        Branch  s(*this);

        s.var.string->append(src,len);

        s.var.length += len;

        return std::move(s);
      }


    Branch  s(data(),length());

    s.var.string->append(src,len);

    s.var.length += len;

    return std::move(s);
  }

  Branch  add(T const&  t) const
  {
    return add(&t,1);
  }

  void  clear()
  {
      if(var.string)
      {
        auto  flag = !var.string->unrefer();

          if(flag)
          {
            auto  id_ = id();

            delete var.string;


            auto  ln = var.link;

              if(ln)
              {
                  if(!ln->reference_count())
                  {
                    delete ln;
printf("Link was deleted by Branch\n");
                  }

                else
                  {
                    ln->unlink();
                  }
              }


printf("String [%zd] was deleted\n",id_);
          }
      }


    var.string = nullptr;
    var.length =       0;
    var.link   = nullptr;
  }

  bool  unique() const{return(var.string->reference_count() == 1);}

  size_t      use_count() const{return var.string? var.string->reference_count():0;}
  size_t  observe_count() const{return var.link? var.link->reference_count():0;}

  uintptr_t  id() const{return var.string? var.string->id():0;}

  T const*  data() const{return var.string->data();}

  T const&  back() const{return var.string->back();}

  T const*  begin() const{return var.string->data()         ;}
  T const*    end() const{return var.string->data()+length();}

  T const*  cbegin() const{return var.string->data()         ;}
  T const*    cend() const{return var.string->data()+length();}

  size_t  length() const{return var.length;}

  Bud  bud()
  {
    auto&  ln = var.link;

      if(!ln)
      {
        ln = new Link(var.string);
      }


    return Bud(*ln,length());
  }

  void  print() const
  {
    printf("\"");

      for(int  i = 0;  i < length();  ++i)
      {
        printf("%c",var.string->at(i));
      }


    printf("\"\n");
  }

};


}




#endif




