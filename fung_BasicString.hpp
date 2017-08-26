#ifndef FUNG_BasicString_HPP_INCLUDED
#define FUNG_BasicString_HPP_INCLUDED


#include<cstddef>
#include<cstdint>
#include<algorithm>


namespace fung{



template<typename  T>
class
BasicString
{
  struct{
    T*  pointer=nullptr;

    size_t            length=0;
    size_t  allocated_length=0;

    size_t  reference_count=0;

  } var;


  void  extend(size_t  n)
  {
      if(n > var.allocated_length)
      {
        auto  new_data = new T[n];

          for(int  i = 0;  i < var.length;  ++i)
          {
            new_data[i] = std::move(var.pointer[i]);
          }


        delete[] var.pointer           ;
                 var.pointer = new_data;

        var.allocated_length = n;
      }
  }


  void  extend__when_over_capacity(size_t  n)
  {
      if(n+1 > var.allocated_length)
      {
        extend(n*2+1);
      }
  }

  void  trancate(size_t  n)
  {
      if(var.length > n)
      {
          for(int  i = n;  i < var.allocated_length;  ++i)
          {
            var.pointer[i] = T();
          }


        var.length = n;
      }
  }

public:
  BasicString(){}

  BasicString(T const*  src)
  {
    append(src);
  }

  BasicString(T const*  src, size_t  len)
  {
    append(src,len);
  }


  BasicString(BasicString const&  rhs) noexcept{*this = rhs;}
  BasicString(BasicString&&       rhs) noexcept{*this = std::move(rhs);}


  ~BasicString(){clear();}


  BasicString&  operator=(BasicString const&  rhs) noexcept
  {
      if(this != &rhs)
      {
        assign(rhs);
      }


    return *this;
  }

  BasicString&  operator=(BasicString&&  rhs) noexcept
  {
      if(this != &rhs)
      {
        clear();

        var.pointer = rhs.var.pointer          ;
                      rhs.var.pointer = nullptr;

        var.length = rhs.var.length    ;
                     rhs.var.length = 0;

        var.allocated_length = rhs.var.allocated_length    ;
                               rhs.var.allocated_length = 0;

        var.reference_count = rhs.var.reference_count    ;
                              rhs.var.reference_count = 0;
      }


    return *this;
  }

  BasicString&  operator+=(BasicString const&  rhs)
  {
    append(rhs);

    return *this;
  }

  BasicString&  operator+=(T const*  src)
  {
    append(src);

    return *this;
  }

  BasicString&  operator+=(T const&  t)
  {
    extend__when_over_capacity(length()+1);


    *(data()+length()) = t;

    var.length += 1;


    return *this;
  }

  BasicString&  operator+=(T&&  t)
  {
    extend__when_over_capacity(length()+1);


    *(data()+length()) = std::move(t);

    var.length += 1;


    return *this;
  }

  BasicString  operator+(BasicString const&  rhs) const
  {
    auto  s = *this;

    s += rhs;

    return std::move(s);
  }

  BasicString  operator+(T const&  t) const
  {
    auto  s = *this;

    s += t;

    return std::move(s);
  }

  BasicString  operator+(T&&  t) const
  {
    auto  s = *this;

    s += std::move(t);

    return std::move(s);
  }


  T&        operator[](int  i)      {return var.pointer[i];}
  T const&  operator[](int  i) const{return var.pointer[i];}


  T&        at(int  i)      {return var.pointer[i];}
  T const&  at(int  i) const{return var.pointer[i];}

  T*        data()      {return var.pointer;}
  T const*  data() const{return var.pointer;}


  void  assign(BasicString<T> const&  rhs)
  {
      if(this != &rhs)
      {
        clear();

        append(rhs);
      }
  }

  void  assign(BasicString<T>&&  rhs)
  {
      if(this != &rhs)
      {
        clear();

        append(std::move(rhs));
      }
  }

  void  assign(T const*  src)
  {
      if(data() != src)
      {
        clear();

        append(src);
      }
  }

  void  assign(T const*  src, size_t  len)
  {
      if(data() != src)
      {
        clear();

        append(src,len);
      }
  }

  void  append(BasicString<T> const&  rhs)
  {
    append(rhs.data(),rhs.length());
  }

  void  append(BasicString<T>&&  rhs)
  {
    extend__when_over_capacity(length()+rhs.length());


    auto  dst = data()+length();
    auto  src =      rhs.data();

    var.length += rhs.length();

    auto  n = rhs.length();

      while(n--)
      {
        *dst++ = std::move(*src++);
      }
  }

  void  append(T const*  src)
  {
    append(src,length(src));
  }

  void  append(T const*  src, size_t  len)
  {
    extend__when_over_capacity(length()+len);


    auto  dst = data()+length();

    var.length += len;

      while(len--)
      {
        *dst++ = *src++;
      }
  }

  void  append(T const&  t)
  {
    append(&t,1);
  }


  size_t    length() const{return var.length;}
  size_t  capacity() const{return var.allocated_length-1;}

  T*  begin(){return var.pointer           ;}
  T*    end(){return var.pointer+var.length;}

  T const*  cbegin(){return var.pointer           ;}
  T const*    cend(){return var.pointer+var.length;}

  void  resize(size_t  n)
  {
      extend(n);
    trancate(n);
  }

  void  clear()
  {
    delete[] var.pointer          ;
             var.pointer = nullptr;

              var.length = 0;
    var.allocated_length = 0;
     var.reference_count = 0;
  }


  void      refer(){       ++var.reference_count;}
  size_t  unrefer(){return --var.reference_count;}

  size_t  reference_count() const{return var.reference_count;}

  void  start_reference_count(){var.reference_count = 1;}

  uintptr_t  id() const{return reinterpret_cast<uintptr_t>(this);}

  static size_t  length(T const*  p)
  {
    auto  len = 0;

      while(*p)
      {
        ++len;
        ++p;
      }


    return len;
  }

};


}




#endif




