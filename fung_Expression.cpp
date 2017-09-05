#include"fung_Expression.hpp"
#include"fung_ExpressionNode.hpp"


namespace fung{




Expression&
Expression::
operator=(Expression const&  rhs) noexcept
{
    if(&rhs != this)
    {
      reset(rhs.ptr? new ExpressionNode(*rhs.ptr):nullptr);
    }


  return *this;
}


Expression&
Expression::
operator=(Expression&&  rhs) noexcept
{
    if(&rhs != this)
    {
      reset(rhs.ptr)         ;
            rhs.ptr = nullptr;
    }


  return *this;
}


Expression&
Expression::
operator=(ExpressionNode&&  nd)
{
  reset(new ExpressionNode(std::move(nd)));

  return *this;
}




void
Expression::
reset(ExpressionNode*  new_ptr)
{
  delete ptr          ;
         ptr = new_ptr;
}


ExpressionNode*
Expression::
release()
{
  auto  node = ptr          ;
               ptr = nullptr;

  return node;
}


}




