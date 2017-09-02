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
  static constexpr bool  debug = false;

  using String = BasicString<T>;

  class Link{
    struct{
      String*  target;

      size_t       reference_count=1;
      size_t  weak_reference_count=0;

    } var;

  public:
    Link(String*  s){var.target = s;}

    operator bool() const{return var.target;}

    uintptr_t  id() const{return reinterpret_cast<uintptr_t>(this);}

    String*  target() const{return var.target;}

    void  refer()
    {
      ++var.reference_count;

        if(debug)
        {
          print();
        }
    }

    void  unrefer()
    {
        if(!--var.reference_count)
        {
          delete var.target          ;
                 var.target = nullptr;
        }


        if(debug)
        {
          print();
        }
    }

    void  refer_weakly()
    {
      ++var.weak_reference_count;

        if(debug)
        {
          print();
        }
    }

    void  unrefer_weakly()
    {
      --var.weak_reference_count;

        if(debug)
        {
          print();
        }
    }

    size_t       reference_count() const{return var.reference_count;}
    size_t  weak_reference_count() const{return var.weak_reference_count;}

    void  print() const
    {
      auto   n =      reference_count();
      auto  wn = weak_reference_count();

      printf("now, Link[%zd] is refered by %d branch%s and %d bud%s\n",id(),n,(n > 1)? "es":"",wn,(wn > 1)? "s":"");
    }
  };

public:
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

      ln.refer_weakly();
    }

    Bud(Bud const&  rhs) noexcept
    {
      var = rhs.var;

      var.link->refer_weakly();
    }

    Bud(Bud&&  rhs)
    {
      var = rhs.var;

      rhs.var.link = nullptr;
    }

   ~Bud()
    {
        if(var.link)
        {
          var.link->unrefer_weakly();


          auto  id = var.link->id();

          auto   n = var.link->reference_count();
          auto  wn = var.link->weak_reference_count();

            if(!n && !wn)
            {
              delete var.link;

                if(debug)
                {
                  printf("Link[%zd] was deleted by Bud\n",id);
                }
            }
        }
    }


    Bud&  operator=(Bud const&  rhs) noexcept=delete;
    Bud&  operator=(Bud&&  rhs) noexcept=delete;

    bool  test() const{return var.link->target();}

    Branch  branch() const{return Branch(var.link,var.length,nullptr,0);}

  };

private:
  struct Var{
    Link*  link=nullptr;

    size_t  length=0;

    Var(                       ){}
    Var(String*  s             ): link(new Link(s                )), length(s->length()){}
    Var(String*  s, size_t  len): link(new Link(s                )), length(len){}
    Var(T const* s, size_t  len): link(new Link(new String(s,len))), length(len){}
    Var(Link*   ln, size_t  len): link(                         ln), length(len){}

  } var;


  Branch(Link*  ln, size_t  len, T const*  add_src, size_t  add_len): var(ln,len)
  {
    ln->target()->append(add_src,add_len);

    var.length += add_len;

    ln->refer();
  }


  Branch(Branch const&  base, T const*  add_src, size_t  add_len): var(base.data(),base.length())
  {
    var.link->target()->append(add_src,add_len);

    var.length += add_len;
  }


public:
  Branch(                            ): var(new String(       ),0){}
  Branch(std::initializer_list<T>  ls): var(new String( ls    )  ){}
  Branch(T const*  src             ):   var(new String(src    )  ){}
  Branch(T const*  src, size_t  len):   var(new String(src,len)){}

  Branch(Branch const&  rhs) noexcept
  {
    var = rhs.var;

    var.link->refer();
  }

  Branch(Branch&&  rhs) noexcept
  {
    var.link = rhs.var.link          ;
               rhs.var.link = nullptr;

    var.length = rhs.var.length    ;
                 rhs.var.length = 0;
  }
 
 ~Branch()
  {
      if(!var.link)
      {
        return;
      }


    auto  id_ = id();

    var.link->unrefer();

      if(!var.link->reference_count())
      {
          if(debug)
          {
            printf("link target was deleted\n");
          }


          if(!var.link->weak_reference_count())
          {
            delete var.link;

              if(debug)
              {
                printf("Link[%zd] was deleted by Branch\n",id_);
              }
          }
      }
  }


  Branch&  operator=(Branch const&  rhs) noexcept=delete;
  Branch&  operator=(Branch&&       rhs) noexcept=delete;


  Branch  operator+(Branch const&  rhs) const{return add(rhs);}
  Branch  operator+(T const*  src)            const{return add(src);}
  Branch  operator+(T const&    t)            const{return add(t);}


  String const&  operator*() const{return *var.link->target();}
  String const*  operator->() const{return var.link->target();}

  String const&  string() const{return *var.link->target();}

  Branch  add(Branch const&  rhs) const{return add(rhs.data(),rhs.length());}
  Branch  add(T const*  src)            const{return add(src,String::length(src));}

  Branch  add(T const*  src, size_t  len) const
  {
      if(length() == string().length())
      {
        return Branch(var.link,length(),src,len);
      }


    return Branch(*this,src,len);
  }

  Branch  add(T const&  t) const
  {
    return add(&t,1);
  }


  bool  unique() const{return(var.link->reference_count() == 1);}

  size_t  id() const{return var.link->id();}

  size_t      use_count() const{return var.link->reference_count();}
  size_t  observe_count() const{return var.link->weak_reference_count();}

  T const*  data() const{return string().data();}

  T const&  back() const{return string().back();}

  T const*  begin() const{return string().data()         ;}
  T const*    end() const{return string().data()+length();}

  T const*  cbegin() const{return string().data()         ;}
  T const*    cend() const{return string().data()+length();}

  size_t  length() const{return var.length;}

  Bud  bud() const
  {
    return Bud(*var.link,length());
  }

  void  print() const
  {
    printf("\"");

      for(auto&  c: *this)
      {
        printf("%c",c);
      }


    printf("\"");
  }

};


}




#endif




