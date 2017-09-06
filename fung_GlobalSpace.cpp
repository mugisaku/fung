#include"fung_GlobalSpace.hpp"
#include"fung_function.hpp"




namespace fung{




GlobalSpace::
GlobalSpace()
{
}




void
GlobalSpace::
append_function(std::string&&  name, Function*  fn)
{
    if(fn)
    {
      function_list.emplace_back(fn);

      auto&  bk = function_list.back();

      append_variable(new Variable(std::string(name),Value(*bk)));
    }
}


void
GlobalSpace::
append_variable(Variable*  var)
{
    if(var)
    {
      variable_list.emplace_back(var);
    }
}


void
GlobalSpace::
append_function_body(FunctionBody*  fnbody)
{
    if(fnbody)
    {
      body_list.emplace_back(fnbody);
    }
}


std::unique_ptr<Variable> const&
GlobalSpace::
find(std::string const&  name) const
{
    for(auto&  v: variable_list)
    {
        if(v->name == name)
        {
          return v;
        }
    }


  static std::unique_ptr<Variable>  null;

  return null;
}


void
GlobalSpace::
print() const
{
  printf("%d個の関数\n",function_list.size());
  printf("%d個の関数本体\n",body_list.size());

    for(auto&  var: variable_list)
    {
      var->print();

      printf("\n");
    }
}


}




