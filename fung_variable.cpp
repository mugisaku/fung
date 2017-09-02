#include"fung_variable.hpp"
#include"fung_function.hpp"
#include"fung_GlobalSpace.hpp"
#include"fung_context.hpp"
#include"fung_error.hpp"




namespace fung{




Constant::
Constant(ValueKind  k, std::string&&  name_, ExpressionList&&  ls):
value_kind(k),
name(std::move(name_)),
expression_list(std::move(ls)),
value(ValueKind::unevaluated)
{
}


Constant::
Constant(std::string&&  name_, Value&&  v):
value_kind(v.get_kind()),
name(std::move(name_)),
value(std::move(v))
{
}




Value const&
Constant::
get_value(std::unique_ptr<GlobalSpace> const&  gsp)
{
    if(value == ValueKind::unevaluated)
    {
      Context  ctx(gsp);

        switch(value_kind)
        {
      case(ValueKind::null):
          break;
      case(ValueKind::unevaluated):
          break;
      case(ValueKind::undefined):
          break;
      case(ValueKind::boolean):
          {
              if(expression_list.size() != 1)
              {
                throw Error("初期化の値がひとつではない");
              }


            value = expression_list.back().evaluate(ctx).convert_to_boolean();
          }
          break;
      case(ValueKind::integer):
          {
              if(expression_list.size() != 1)
              {
                throw Error("初期化の値がひとつではない");
              }


            value = expression_list.back().evaluate(ctx).convert_to_integer();
          }
          break;
      case(ValueKind::function):
          break;
      case(ValueKind::string):
          {
              if(expression_list.size() != 1)
              {
                throw Error("初期化の値がひとつではない");
              }


            value = expression_list.back().evaluate(ctx).convert_to_string();
          }
          break;
      case(ValueKind::list):
          {
            value = Value(to_list(expression_list,ctx));
          }
          break;
      case(ValueKind::any):
          break;
        }
    }


  return value;
}


void
Constant::
print() const
{
  printf("%s %s",Value::to_string(value_kind).data(),name.data());

    if(value == ValueKind::function)
    {
      value->function->print();
    }

  else
    {
      auto  i = expression_list.cbegin();
      auto  e = expression_list.cend();

      printf("(");

        if(i != e)
        {
          i++->print();

            while(i != e)
            {
              printf(",");

              i++->print();
            }
        }


      printf(")");
    }
}


}




