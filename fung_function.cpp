#include"fung_function.hpp"
#include"fung_error.hpp"




namespace fung{


Value
Function::
operator()(Context&  ctx, std::initializer_list<Value>  args) const
{
    if(parameter_list.size() != args.size())
    {
      throw Error("引数の数が一致しない");
    }


  Frame  frame(*this);

  auto  it = args.begin();

    for(auto&  para: parameter_list)
    {
        if(para.get_value_kind() != it->get_kind())
        {
          throw Error("引数の型が一致しない");
        }


      frame.append(para.get_name(),*it++);
    }


  ctx.push_frame(std::move(frame));

    for(auto&  stmt: body)
    {
        if(stmt == StatementKind::return_)
        {
          return stmt->evaluate(ctx);
        }
    }


  ctx.pop_frame();

  return Value();
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
    }


  printf("\n}\n\n");
}


}




