#ifndef FUNG_SharedString_HPP_INCLUDED
#define FUNG_SharedString_HPP_INCLUDED


#include"fung_BasicString.hpp"
#include<cstdio>


namespace fung{



template<typename  T>
class
SharedString
{
  struct{
    BasicString<T>*  string;

    size_t  length;

  } var;

public:
  SharedString(BasicString<T>*  s=nullptr)
  {
    var.string = s               ;
    var.length = s? s->length():0;

      if(s)
      {
        s->start_reference_count();
      }
  }

  SharedString(T const*  src)
  {
    var.string = new BasicString<T>(src);
    var.length = var.string->length();

    var.string->start_reference_count();
  }

  SharedString(T const*  src, size_t  len)
  {
    var.string = new BasicString<T>(src.len);
    var.length = len;

    var.string->start_reference_count();
  }

  SharedString(SharedString const&  rhs) noexcept
  {
    var.string = rhs.var.string;
    var.length = rhs.var.length;

      if(var.string)
      {
        var.string->refer();
//printf("REFCOUNT %zd\n",var.string->reference_count());
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


  BasicString<T> const&  operator*() const{return *var.string;}
  BasicString<T> const*  operator->() const{return var.string;}


  SharedString  add(SharedString const&  rhs) const
  {
    return add(rhs.data().rhs.length());
  }

  SharedString  add(T const*  src) const
  {
    return add(src,BasicString<T>::length(src));
  }

  SharedString  add(T const*  src, size_t  len) const
  {
      if(unique() || (length() == var.string->length()))
      {
        SharedString<T>  s(*this);

        s.var.string->append(src,len);

        s.var.length = s.var.string->length();

        return std::move(s);
      }


    SharedString<T>  s(new BasicString<T>(var.string->data(),length()));

    s.var.string->append(src,len);

    s.var.length = s.var.string->length();

    return std::move(s);
  }

  SharedString  add(T const&  t) const
  {
      if(unique() || (length() == var.string->length()))
      {
        SharedString<T>  s(*this);

        s.var.string->append(t);

        s.var.length += 1;

        return std::move(s);
      }


    SharedString<T>  s(new BasicString<T>(var.string->data(),length()));

    s.var.string->append(t);

    s.var.length += 1;

    return std::move(s);
  }

  void  clear()
  {
      if(var.string)
      {
        auto  flag = !var.string->unrefer();

//printf("REFCOUNT %zd\n",var.string->reference_count());
          if(flag)
          {
//printf("delete\n");
            delete var.string;
          }
      }


    var.string = nullptr;
    var.length =       0;
  }

  bool  unique() const{return(var.string->reference_count() == 1);}

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




