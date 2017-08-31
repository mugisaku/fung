#ifndef FUNG_BasicString_HPP_INCLUDED
#define FUNG_BasicString_HPP_INCLUDED


#include<cstdint>
#include<algorithm>
#include<initializer_list>


namespace fung{



template<typename  T>
class
BasicString
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


  struct{
    T*  pointer=nullptr;

    size_t            length=0;

    size_t  reference_count=0;

  } var;

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

    var.length          = rhs.length();
    var.reference_count = rhs.reference_count();

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

    var.length          = rhs.length();
    var.reference_count = rhs.reference_count();

    return *this;
  }

  BasicString&  operator=(std::initializer_list<T>  ls) noexcept
  {
    assign(ls);

    return *this;
  }

  bool  operator==(BasicString const&  rhs) const
  {
    return compare(rhs);
  }

  bool  operator==(T const*  buf) const
  {
    return compare(buf);
  }

  bool  compare(BasicString const&  rhs) const
  {
    return compare(rhs.data(),rhs.length());
  }

  bool  compare(T const*  buf) const
  {
    return compare(buf,length(buf));
  }

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

  uintptr_t  id() const{return reinterpret_cast<uintptr_t>(this);}

  void  clear()
  {
    delete[] var.pointer          ;
             var.pointer = nullptr;

    var.length = 0;
    var.reference_count = 0;
  }

  T const*  data() const{return var.pointer;}

  T const*  begin() const{return var.pointer           ;}
  T const*    end() const{return var.pointer+var.length;}

  T const*  cbegin() const{return var.pointer           ;}
  T const*    cend() const{return var.pointer+var.length;}

  T const&  front() const{return *(var.pointer             );}
  T const&   back() const{return *(var.pointer+var.length-1);}

  T const&  operator[](size_t  i) const{return var.pointer[i];}
  T const&  at(size_t  i) const{return var.pointer[i];}

  size_t  length() const{return var.length;}
//  size_t  capacity() const{return var.allocated_length-1;}

  void  assign(BasicString const&  rhs)
  {
    assign(rhs.data(),rhs.length());
  }

  void  assign(std::initializer_list<T>  ls)
  {
    assign(ls.begin(),ls.size());
  }

  void  assign(T const*  buf)
  {
    assign(buf,length(buf));
  }

  void  assign(T const*  buf, size_t  len)
  {
    clear();

    append(buf,len);
  }

  void  append(BasicString const&  rhs)
  {
    append(rhs.data(),rhs.length());
  }

  void  append(std::initializer_list<T>  ls)
  {
    append(ls.begin(),ls.size());
  }

  void  append(T const*  buf)
  {
    append(buf,length(buf));
  }

  void  append(T const*  buf, size_t  len)
  {
    auto  old_len = length();

    var.length = old_len+len;


    auto  new_data = new T[var.length+1];

    auto  dst = new_data;

    auto  src = var.pointer           ;
                var.pointer = new_data;

      while(old_len--)
      {
        *dst++ = std::move(*src++);
      }


      while(len--)
      {
        *dst++ = std::move(*buf++);
      }


    *dst = T();
  }

  void  refer()
  {
    ++var.reference_count;
  }

  size_t  unrefer()
  {
    return --var.reference_count;
  }

  void  start_reference_count()
  {
    var.reference_count = 1;
  }

  size_t  reference_count() const
  {
    return var.reference_count;
  }

};



}




#endif




