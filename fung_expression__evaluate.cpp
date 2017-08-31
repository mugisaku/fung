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
      auto  b = lv.convert_to_boolean();

        if(b.is_undefined())
        {
          throw Error("論理値が未定義");
        }


      return right->evaluate(ctx,b->boolean);
    }

  else
    if(mnemonic == Mnemonic::log_or)
    {
      auto  b = lv.convert_to_boolean();

        if(b.is_undefined())
        {
          throw Error("論理値が未定義");
        }


        if(b->boolean)
        {
          return Value(true);
        }


      auto  rv = right->evaluate(ctx);

      return rv.convert_to_boolean();
    }

  else
    if(mnemonic == Mnemonic::log_and)
    {
      auto  b = lv.convert_to_boolean();

        if(b.is_undefined())
        {
          throw Error("論理値が未定義");
        }


        if(!b->boolean)
        {
          return Value(false);
        }


      auto  rv = right->evaluate(ctx);

      return rv.convert_to_boolean();
    }



       if(mnemonic == Mnemonic::neg    ){return Value::neg(    lv);}
  else if(mnemonic == Mnemonic::log_not){return Value::log_not(lv);}
  else if(mnemonic == Mnemonic::bit_not){return Value::bit_not(lv);}


    if(mnemonic == Mnemonic::cal)
    {
      static std::string  const fn_name("匿名関数");

        if(lv != ValueKind::function)
        {
          throw Error("関数呼び出しの左辺が関数参照ではない");
        }


      auto  rv = right->evaluate(ctx);

        if(rv != ValueKind::list)
        {
          throw Error("関数呼び出しの右辺が引数リストではない");
        }


      auto  flag = left->get_kind() == ExpressionKind::identifier;      

      auto&  name = flag? (*left)->identifier.string:fn_name;

      return (*lv->function)(name,ctx,rv->list);
    }


    if(mnemonic == Mnemonic::acc)
    {
        if(right->get_kind() != ExpressionKind::identifier)
        {
          throw Error("メンバー呼び出しの右辺が識別子ではない");
        }


      return lv.get_property((*right)->identifier.string);
    }


  auto  rv = right->evaluate(ctx);

    if(rv == ValueKind::list)
    {
      rv = rv->list.back();
    }


    switch(mnemonic)
    {
  case(Mnemonic::add): return Value::add(lv,rv);
  case(Mnemonic::sub): return Value::sub(lv,rv);
  case(Mnemonic::mul): return Value::mul(lv,rv);
  case(Mnemonic::div):
        if(!rv)
        {
          throw Error(Cursor(),"ゼロ除算\n");
        }


      return Value::div(lv,rv);
      break;
  case(Mnemonic::rem):
        if(!rv)
        {
          throw Error(Cursor(),"ゼロ除算\n");
        }


      return Value::rem(lv,rv);
      break;
  case(Mnemonic::shl    ): return Value::shl(    lv,rv);
  case(Mnemonic::shr    ): return Value::shr(    lv,rv);
  case(Mnemonic::bit_and): return Value::bit_and(lv,rv);
  case(Mnemonic::bit_or ): return Value::bit_or( lv,rv);
  case(Mnemonic::bit_xor): return Value::bit_xor(lv,rv);
  case(Mnemonic::eq     ): return Value::eq(     lv,rv);
  case(Mnemonic::neq    ): return Value::neq(    lv,rv);
  case(Mnemonic::lt     ): return Value::lt(     lv,rv);
  case(Mnemonic::lteq   ): return Value::lteq(   lv,rv);
  case(Mnemonic::gt     ): return Value::gt(     lv,rv);
  case(Mnemonic::gteq   ): return Value::gteq(   lv,rv);
  case(Mnemonic::sus):
      {
      }
      break;
  default:
      throw Error(Cursor(),"");
    }


  return undefined;
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
  case(ExpressionKind::list):
      return Value(to_value_list(data.list,ctx));
      break;
    }


  return undefined;
}




ValueList
read_list(ValueList  ls, ExpressionList::const_iterator  it, ExpressionList::const_iterator  end, Context&  ctx)
{
  return(it != end)? read_list(ls+it->evaluate(ctx),it+1,end,ctx):ls;
}


ValueList
to_value_list(ExpressionList const&  ls, Context&  ctx)
{

  return read_list(ValueList(),ls.cbegin(),ls.cend(),ctx);
}




}




