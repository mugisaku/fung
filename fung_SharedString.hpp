#ifndef FUNG_SharedString_HPP_INCLUDED
#define FUNG_SharedString_HPP_INCLUDED


#include<string>
#include<cstdio>
#include<cstdint>


namespace fung{



template<typename  T>
class
SharedString
{
  static size_t  length(T const*  s)
  {
    size_t  n = 0;

      while(*s)
      {
        ++n;
        ++s;
      }


    return n;
  }


  template<typename  U>
  class String: public std::basic_string<U>{
    size_t  reference_count=0;

  public:
    String(std::basic_string<U>&&  src): std::string(std::move(src)){}

    uintptr_t  id() const{return reinterpret_cast<uintptr_t>(this);}

    void  refer()
    {
      ++reference_count;
    }

    bool  unrefer()
    {
      return --reference_count;
    }

    void  start_reference_count()
    {
      reference_count = 1;
    }

    size_t  get_reference_count()
    {
      return reference_count;
    }

  };


  struct{
    String<T>*  string;

    size_t  length;

  } var;


public:
  SharedString(T const*  src)
  {
    var.string = new String<T>(std::string(src));
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  SharedString(T const*  src, size_t  len)
  {
    var.string = new String<T>(std::string(src,len));
    var.length = len;

    var.string->start_reference_count();
  }

  SharedString(std::basic_string<T>&&  s)
  {
    var.string = new String<T>(std::move(s));
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


  std::basic_string<T> const&  operator*() const{return *var.string;}
  std::basic_string<T> const*  operator->() const{return var.string;}


  SharedString  add(SharedString const&  rhs) const
  {
    return add(rhs.data(),rhs.length());
  }

  SharedString  add(T const*  src) const
  {
    return add(src,length(src));
  }

  SharedString  add(T const*  src, size_t  len) const
  {
      if(length() == var.string->length())
      {
        SharedString<T>  s(*this);

        s.var.string->append(src,len);

        s.var.length += len;

        return std::move(s);
      }


    SharedString<T>  s(std::string(var.string->data(),length()));

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
            auto  id = var.string->id();

            delete var.string;

            printf("%zd was deleted\n",id);
          }
      }


    var.string = nullptr;
    var.length =       0;
  }

  bool  unique() const{return(var.string->reference_count() == 1);}

  size_t  use_count() const{return var.string? var.string->get_reference_count():0;}

  uintptr_t  id() const{return var.string? var.string->id():0;}

  T const*  data() const{return var.string->data();}

  size_t  length() const{return var.length;}

  void  print() const
  {
      for(int  i = 0;  i < var.length;  ++i)
      {
        printf("%c",var.string->at(i));
      }
  }

};


}




#endif




