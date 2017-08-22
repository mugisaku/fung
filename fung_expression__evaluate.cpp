#include"fung_expression.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"
#include"fung_context.hpp"




namespace fung{




Value
Expression::
operate(Context&  ctx, bool  b) const
{
  auto&  mnemonic = data.mnemonic;

    if(mnemonic == Mnemonic::eth)
    {
        if(b){return  left->evaluate(ctx);}
      else   {return right->evaluate(ctx);}
    }

  
  auto  lv = left->evaluate(ctx);

    if(mnemonic == Mnemonic::cho)
    {
      auto  b = lv.to_boolean();

        if(b.is_undefined())
        {
          throw Error("論理値が未定義");
        }


      return right->evaluate(ctx,b->boolean);
    }

  else
    if(mnemonic == Mnemonic::log_or)
    {
      auto  b = lv.to_boolean();

        if(b.is_undefined())
        {
          throw Error("論理値が未定義");
        }


        if(b->boolean)
        {
          return Value(true);
        }


      auto  rv = right->evaluate(ctx);

      return rv.to_boolean();
    }

  else
    if(mnemonic == Mnemonic::log_and)
    {
      auto  b = lv.to_boolean();

        if(b.is_undefined())
        {
          throw Error("論理値が未定義");
        }


        if(!b->boolean)
        {
          return Value(false);
        }


      auto  rv = right->evaluate(ctx);

      return rv.to_boolean();
    }



       if(mnemonic == Mnemonic::neg    ){return -lv;}
  else if(mnemonic == Mnemonic::log_not){return !lv;}
  else if(mnemonic == Mnemonic::bit_not){return ~lv;}


    if(mnemonic == Mnemonic::cal)
    {
        if(lv != ValueKind::function)
        {
          throw Error("関数呼び出しの左辺が関数参照ではない");
        }


        if((*right) != ExpressionKind::list)
        {
          throw Error("関数呼び出しの右辺が引数リストではない");
        }


      

      return (*lv->function)(ctx,right->data.list);
    }


  auto  rv = right->evaluate(ctx);

    switch(mnemonic)
    {
  case(Mnemonic::add): return lv+rv;
  case(Mnemonic::sub): return lv-rv;
  case(Mnemonic::mul): return lv*rv;
  case(Mnemonic::div):
        if(!rv)
        {
          throw Error(Cursor(),"ゼロ除算\n");
        }


      return lv/rv;
      break;
  case(Mnemonic::rem):
        if(!rv)
        {
          throw Error(Cursor(),"ゼロ除算\n");
        }


      return lv%rv;
      break;
  case(Mnemonic::shl    ): return lv<<rv;
  case(Mnemonic::shr    ): return lv>>rv;
  case(Mnemonic::bit_and): return lv&rv;
  case(Mnemonic::bit_or ): return lv|rv;
  case(Mnemonic::bit_xor): return lv^rv;
  case(Mnemonic::eq     ): return lv == rv;
  case(Mnemonic::neq    ): return lv != rv;
  case(Mnemonic::lt     ): return lv <  rv;
  case(Mnemonic::lteq   ): return lv <= rv;
  case(Mnemonic::gt     ): return lv >  rv;
  case(Mnemonic::gteq   ): return lv >= rv;
  case(Mnemonic::sus):
      {
      }
      break;
  default:
      throw Error(Cursor(),"");
    }


  return Value(Undefined());
}


Value
Expression::
evaluate(Context&  ctx, bool  b) const
{
    switch(kind)
    {
  case(ExpressionKind::null):
  case(ExpressionKind::operator_):
      throw Error("未定義の値");
      break;
  case(ExpressionKind::operation):
      return operate(ctx,b);
      break;
  case(ExpressionKind::value):
      return data.value;
      break;
  case(ExpressionKind::identifier):
      return ctx[data.identifier.string];
      break;
    }


  return Value();
}


}




