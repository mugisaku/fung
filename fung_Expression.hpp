#ifndef FUNG_Expression_HPP_INCLUDED
#define FUNG_Expression_HPP_INCLUDED


#include<algorithm>


namespace fung{




class ExpressionNode;


class
Expression
{
  ExpressionNode*  ptr=nullptr;

public:
  Expression(ExpressionNode*   new_ptr=nullptr): ptr(new_ptr){}
  Expression(ExpressionNode&&  nd){*this = std::move(nd);}
  Expression(Expression const&  rhs) noexcept{*this = rhs;}
  Expression(Expression&&       rhs) noexcept{*this = std::move(rhs);}
 ~Expression(){reset();}

  Expression&  operator=(Expression const&  rhs) noexcept;
  Expression&  operator=(Expression&&  rhs) noexcept;
  Expression&  operator=(ExpressionNode&&  node);

  operator bool() const{return ptr;}

  ExpressionNode const*  operator->() const{return  ptr;}
  ExpressionNode const&  operator *() const{return *ptr;}

  void  reset(ExpressionNode*  new_ptr=nullptr);

  ExpressionNode*  release();


};


}




#endif




