#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{


Value
Function::
operator()(Context&  ctx, std::initializer_list<Value>  args) const
{
  std::vector<Value>  buf = args;

  return (*this)(ctx,buf);
}


Value
Function::
operator()(Context&  ctx, ExpressionList const&  args) const
{
  std::vector<Value>  buf;

    for(auto&  expr: args)
    {
      buf.emplace_back(expr.evaluate(ctx));
    }


  return (*this)(ctx,buf);
}


Value
Function::
operator()(Context&  ctx, std::vector<Value> const&  args) const
{
    if(parameter_list.size() != args.size())
    {
      throw Error("引数の数が一致しない");
    }


  ctx.enter(*this);

  auto  it = args.begin();

    for(auto&  para: parameter_list)
    {
        if(para.get_value_kind() != it->get_kind())
        {
          throw Error("引数の型が一致しない");
        }


      ctx.entry(para.get_name(),*it++);
    }


    for(auto&  stmt: body)
    {
        if(!stmt.execute(ctx))
        {
          break;
        }
    }


  ctx.leave();

  return ctx.get_returned_value();
}


void
Function::
print() const
{
  printf("function %s(",name.data());

  auto        it = parameter_list.cbegin();
  auto const end = parameter_list.cend();

    if(it != end)
    {
      it++->print();

        while(it != end)
        {
          printf(",");

          it++->print();
        }
    }


  printf(")->%s\n{\n",return_type.data());

    for(auto&  stmt: body)
    {
      stmt.print();

      printf("\n");
    }


  printf("}\n\n");
}


}




