#ifndef FUNG_TinyString_HPP_INCLUDED
#define FUNG_TinyString_HPP_INCLUDED




namespace fung{


struct
TinyString
{
  char  codes[4];

  constexpr TinyString(char  c0=0, char  c1=0, char  c2=0, char  c3=0): codes{c0,c1,c2,c3}{}

  constexpr int  length() const{return !codes[0]? 0:
                                       !codes[1]? 1:
                                       !codes[2]? 2:3;}

  constexpr uint32_t  operator*() const{return((codes[0]<<24)|
                                               (codes[1]<<16)|
                                               (codes[2]<< 8)|
                                               (codes[3]    ));}

  constexpr bool  compare(char  c0                    ) const{return(codes[0] == c0)                                        ;}
  constexpr bool  compare(char  c0, char  c1          ) const{return(codes[0] == c0) && (codes[1] == c1)                    ;}
  constexpr bool  compare(char  c0, char  c1, char  c2) const{return(codes[0] == c0) && (codes[1] == c1) && (codes[2] == c2);}

};


}




#endif




