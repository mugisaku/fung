#ifndef FUNG_SharedString_HPP_INCLUDED
#define FUNG_SharedString_HPP_INCLUDED


#include"fung_BasicString.hpp"
#include<cstdio>
#include<cstdint>
#include<initializer_list>


namespace fung{



template<typename  T>
class
SharedString
{
  using String = BasicString<T>;

  struct{
    String*  string;

    size_t  length;

  } var;


public:
  SharedString()
  {
    var.string = new String();
    var.length = 0;

    var.string->start_reference_count();
  }

  SharedString(std::initializer_list<T>  ls)
  {
    var.string = new String(ls);
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  SharedString(T const*  src)
  {
    var.string = new String(src);
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  SharedString(T const*  src, size_t  len)
  {
    var.string = new String(src,len);
    var.length = len;

    var.string->start_reference_count();
  }

  SharedString(String&&  s)
  {
    var.string = new String(std::move(s));
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  SharedString(SharedString const&  rhs) noexcept
  {
    var.string = rhs.var.string;
    var.length = 0;

      if(var.string)
      {
        var.length = var.string->length();

        var.string->refer();
      }
  }

  SharedString(SharedString&&  rhs) noexcept
  {
    var.string = rhs.var.string          ;
                 rhs.var.string = nullptr;

    var.length = rhs.var.length    ;
                 rhs.var.length = 0;
  }

 ~SharedString(){clear();}

  SharedString&  operator=(SharedString const&  rhs) noexcept=delete;
  SharedString&  operator=(SharedString&&       rhs) noexcept=delete;


  SharedString  operator+(SharedString const&  rhs) const
  {
    return add(rhs);
  }

  SharedString  operator+(T const*  src) const
  {
    return add(src);
  }

  SharedString  operator+(T const&  t) const
  {
    return add(t);
  }


  String const&  operator*() const{return *var.string;}
  String const*  operator->() const{return var.string;}


  SharedString  add(SharedString const&  rhs) const
  {
    return add(rhs.data(),rhs.length());
  }

  SharedString  add(T const*  src) const
  {
    return add(src,String::length(src));
  }

  SharedString  add(T const*  src, size_t  len) const
  {
      if(length() == var.string->length())
      {
        SharedString  s(*this);

        s.var.string->append(src,len);

        s.var.length += len;

        return std::move(s);
      }


    SharedString  s(data(),length());

    s.var.string->append(src,len);

    s.var.length += len;

    return std::move(s);
  }

  SharedString  add(T const&  t) const
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

//            printf("%zd was deleted\n",id_);
          }
      }


    var.string = nullptr;
    var.length =       0;
  }

  bool  unique() const{return(var.string->reference_count() == 1);}

  size_t  use_count() const{return var.string? var.string->reference_count():0;}

  uintptr_t  id() const{return var.string? var.string->id():0;}

  T const*  data() const{return var.string->data();}

  T const&  back() const{return var.string->back();}

  T const*  begin() const{return var.string->data()         ;}
  T const*    end() const{return var.string->data()+length();}

  T const*  cbegin() const{return var.string->data()         ;}
  T const*    cend() const{return var.string->data()+length();}

  size_t  length() const{return var.length;}

  void  print() const
  {
    printf("\"");

      for(int  i = 0;  i < length();  ++i)
      {
        printf("%c",var.string->at(i));
      }


    printf("\"");
  }

};


}




#endif




