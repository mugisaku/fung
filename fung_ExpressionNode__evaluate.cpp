#include"fung_ExpressionNode.hpp"
#include"fung_function.hpp"
#include"fung_error.hpp"
#include"fung_context.hpp"




namespace fung{




Value
ExpressionNode::
tail_call(Context&  ctx) const
{
    if((*left  == ExpressionNodeKind::identifier) &&
       (*right == ExpressionNodeKind::value))
    {
      auto&  id = (*left )->identifier.string;
      auto   rv = (*right).evaluate(ctx,true);

      auto&  function_name = ctx->front().get_function_name();

        if((id == function_name) && (rv == ValueKind::list))
        {
          return Value(TailCalling(std::move(rv->list)));
        }
    }


  return cal(ctx,left->evaluate(ctx,true));
}


Value
ExpressionNode::
choice(Context&  ctx, Value const&  lv, bool  multi) const
{
  auto  b = lv.convert_to_boolean();

    if(b.is_undefined())
    {
      throw Error("論理値が未定義");
    }


    if(*right != ExpressionNodeKind::paired)
    {
      throw Error("条件演算の右辺が@airedでない");
    }


  return b->boolean?  right->get_left()->evaluate(ctx,multi)
                   : right->get_right()->evaluate(ctx,multi);
}


Value
ExpressionNode::
log_or(Context&  ctx, Value const&  lv) const
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


  auto  rv = right->evaluate(ctx,true);

  return rv.convert_to_boolean();
}


Value
ExpressionNode::
log_and(Context&  ctx, Value const&  lv) const
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


  auto  rv = right->evaluate(ctx,true);

  return rv.convert_to_boolean();
}


Value
ExpressionNode::
cal(Context&  ctx, Value const&  lv) const
{
  static std::string  const fn_name("無名関数");

  auto  rv = right->evaluate(ctx,true);

    if(rv != ValueKind::list)
    {
      throw Error("関数呼び出しの右辺が引数リストではない");
    }


    if(lv != ValueKind::function)
    {
      throw Error("関数呼び出しの左辺が関数参照ではない");
    }


  auto  flag = left->get_kind() == ExpressionNodeKind::identifier;      

  auto&  name = flag? (*left)->identifier.string:fn_name;

  return ctx.call(name,*lv->function,std::move(rv->list));
}


Value
ExpressionNode::
sus(Context&  ctx, Value const&  lv) const
{
  auto  rv = right->evaluate(ctx,true).convert_to_integer();

    if(rv != ValueKind::integer)
    {
      throw Error("添字が整数でない");
    }


  return Value::sus(lv,rv->integer);
}


Value
ExpressionNode::
binop(Context&  ctx, Value const&  lv, Value const&  rv) const
{
    switch(data.mnemonic)
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
  default:
      throw Error(Cursor(),"");
    }


  return undefined;
}


Value
ExpressionNode::
operate(Context&  ctx, bool  multi) const
{
  auto&  mnemonic = data.mnemonic;

    if(multi && (mnemonic == Mnemonic::cal))
    {
      return tail_call(ctx);
    }


  auto  lv = left->evaluate(ctx,true);

    if(mnemonic == Mnemonic::cho)
    {
      return choice(ctx,lv,multi);
    }

  else
    if(mnemonic == Mnemonic::log_or)
    {
      return log_or(ctx,lv);
    }

  else
    if(mnemonic == Mnemonic::log_and)
    {
      return log_and(ctx,lv);
    }



       if(mnemonic == Mnemonic::neg    ){return Value::neg(    lv);}
  else if(mnemonic == Mnemonic::log_not){return Value::log_not(lv);}
  else if(mnemonic == Mnemonic::bit_not){return Value::bit_not(lv);}
  else if(mnemonic == Mnemonic::der    ){return Value::der(    lv);}

    if(mnemonic == Mnemonic::cal)
    {
      return cal(ctx,lv);
    }


    if(mnemonic == Mnemonic::acc)
    {
        if(right->get_kind() != ExpressionNodeKind::identifier)
        {
          throw Error("メンバー呼び出しの右辺が識別子ではない");
        }


      return lv.get_property((*right)->identifier.string);
    }


    if(mnemonic == Mnemonic::sus)
    {
      return sus(ctx,lv);
    }


  auto  rv = right->evaluate(ctx,true);

    if(rv == ValueKind::list)
    {
      rv = rv->list.back();
    }


  return binop(ctx,lv,rv);
}


Value
ExpressionNode::
evaluate(Context&  ctx, bool  multi) const
{
    switch(kind)
    {
  case(ExpressionNodeKind::null):
  case(ExpressionNodeKind::operator_):
      throw Error("未定義の値");
      break;
  case(ExpressionNodeKind::paired):
      throw Error("pairedがここで評価されてはいけない");
      break;
  case(ExpressionNodeKind::operation):
      return operate(ctx,multi);
      break;
  case(ExpressionNodeKind::value):
      return data.value;
      break;
  case(ExpressionNodeKind::identifier):
      return ctx[data.identifier.string];
      break;
  case(ExpressionNodeKind::list):
      return Value(to_list(data.list,ctx));
      break;
  case(ExpressionNodeKind::function_body):
      return ctx.call(*data.function_body);
      break;
    }


  return undefined;
}




List
read_list(List  ls, ExpressionList::const_iterator  it, ExpressionList::const_iterator  end, Context&  ctx)
{
  return(it != end)? read_list(ls+(*it)->evaluate(ctx),it+1,end,ctx):ls;
}


List
to_list(ExpressionList const&  ls, Context&  ctx)
{

  return read_list(List(),ls.cbegin(),ls.cend(),ctx);
}




}




