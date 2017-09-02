#ifndef FUNG_BasicString_HPP_INCLUDED
#define FUNG_BasicString_HPP_INCLUDED


#include<cstdint>
#include<cstdio>
#include<algorithm>
#include<initializer_list>


namespace fung{



template<typename  T>
class
BasicString
{
  struct{
    T*  pointer=nullptr;

    size_t    length=0;
    size_t  capacity=0;

  } var;


  void  extend(size_t  new_capacity)
  {
    auto  new_data = new T[new_capacity+1];

    auto  dst = new_data;

    auto  src = var.pointer           ;
                var.pointer = new_data;

    auto  n = length();

      while(n--)
      {
        *dst++ = std::move(*src++);
      }


    var.capacity = new_capacity;
  }

public:
  BasicString(){}
  BasicString(std::initializer_list<T>  ls){assign(ls);}
  BasicString(T const*  buf             ){assign(buf    );}
  BasicString(T const*  buf, size_t  len){assign(buf,len);}
  BasicString(BasicString const&  rhs) noexcept{*this = rhs;}
  BasicString(BasicString&&       rhs) noexcept{*this = std::move(rhs);}
 ~BasicString(){clear();}

  BasicString&  operator=(BasicString const&  rhs) noexcept
  {
    clear();

    var.pointer = new T[rhs.length()+1];

    var.length   = rhs.length();
    var.capacity = rhs.length();

    auto  dst =     data();
    auto  src = rhs.data();

    auto  n = length();

      while(n--)
      {
        *dst++ = *src++;
      }


    return *this;
  }

  BasicString&  operator=(BasicString&&  rhs) noexcept
  {
    clear();

    var.pointer = rhs.var.pointer          ;
                  rhs.var.pointer = nullptr;

    var.length   = rhs.length();
    var.capacity = rhs.capacity();

    return *this;
  }

  BasicString&  operator=(std::initializer_list<T>  ls) noexcept
  {
    assign(ls);

    return *this;
  }


  BasicString&  operator+=(BasicString const&  rhs)     {  append( rhs);  return *this;}
  BasicString&  operator+=(std::initializer_list<T>  ls){  append(  ls);  return *this;}
  BasicString&  operator+=(T const*  src)               {  append( src);  return *this;}
  BasicString&  operator+=(T const&  t)                 {  append(&t,1);  return *this;}


  BasicString  operator+(BasicString const&  rhs)      const{return add(rhs);}
  BasicString  operator+(std::initializer_list<T>  ls) const{return add(ls);}
  BasicString  operator+(T const*  src)                const{return add(src);}
  BasicString  operator+(T const&    c)                const{return add(&c,1);}


  bool  operator==(BasicString const&  rhs) const{return compare(rhs);}
  bool  operator==(T const*  buf)           const{return compare(buf);}

  bool  compare(BasicString const&  rhs) const{return compare(rhs.data(),rhs.length());}
  bool  compare(T const*  buf)           const{return compare(       buf, length(buf));}

  bool  compare(T const*  buf, size_t  len) const
  {
      if(length() != len)
      {
        return false;
      }


    auto  dst = data();

      while(len--)
      {
          if(*dst++ != *buf++)
          {
            return false;
          }
      }


    return true;
  }

  void  clear()
  {
    delete[] var.pointer          ;
             var.pointer = nullptr;

    var.length   = 0;
    var.capacity = 0;
  }

  T      *  data()      {return var.pointer;}
  T const*  data() const{return var.pointer;}

  T*  begin() const{return var.pointer           ;}
  T*    end() const{return var.pointer+var.length;}

  T const*  cbegin() const{return var.pointer           ;}
  T const*    cend() const{return var.pointer+var.length;}

  T const&  front() const{return *(var.pointer             );}
  T const&   back() const{return *(var.pointer+var.length-1);}

  T const&  operator[](size_t  i) const{return var.pointer[i];}
  T const&  at(size_t  i) const{return var.pointer[i];}

  size_t    length() const{return var.length;}
  size_t  capacity() const{return var.capacity;}


  void  assign(BasicString const&  rhs)     {assign(rhs.data(),rhs.length());}
  void  assign(std::initializer_list<T>  ls){assign(ls.begin(),ls.size());}
  void  assign(T const*  buf)               {assign(buf,length(buf));
  }

  void  assign(T const*  buf, size_t  len)
  {
    clear();

    append(buf,len);
  }


  void  append(BasicString const&  rhs)     {append(rhs.data(),rhs.length());}
  void  append(std::initializer_list<T>  ls){append(ls.begin(),ls.size());}
  void  append(T const*  buf)               {append(buf,length(buf));}

  void  append(T const*  buf, size_t  len)
  {
    auto  new_length = (length()+len);

      if(new_length > capacity())
      {
        extend(new_length*2);
      }


    auto  dst = end();

    var.length += len;

      while(len--)
      {
        *dst++ = *buf++;
      }
  }


  BasicString  add(BasicString const&  rhs)      const{return add(rhs.data(),rhs.length());}
  BasicString  add(std::initializer_list<T>  ls) const{return add(ls.begin(),ls.size());}
  BasicString  add(T const*  buf)                const{return add(buf,length(buf));}

  BasicString  add(T const*  buf, size_t  len) const
  {
    BasicString  s(*this);

    s.append(buf,len);

    return std::move(s);
  }


  uintptr_t  id() const{return reinterpret_cast<uintptr_t>(this);}

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



};



}




#endif




