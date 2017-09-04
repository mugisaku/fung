#include"fung_ExecutionFrame.hpp"




namespace fung{




ExecutionFrame::
ExecutionFrame(std::string const&  fn_name, Function const&  fn):
function_name(fn_name),
function(&fn),
current(fn.get_body().cbegin()),
end(fn.get_body().cend())
{
}


ExecutionFrame::
ExecutionFrame(StatementList const&  stmtls):
function_name(""),
function(nullptr),
current(stmtls.cbegin()),
end(stmtls.cend())
{
}




void
ExecutionFrame::
append_variable(Variable&&  var)
{
   for(auto&  v: variable_list)
   {
       if(v.name == var.name)
       {
         throw Error("すでに%sという名前の変数がある",var.name.data());
       }
   }


  variable_list.emplace_back(std::move(var));
}


Variable const*
ExecutionFrame::
find_variable(std::string const&  name) const
{
    for(auto&  var: variable_list)
    {
        if(var.name == name)
        {
          return &var;
        }
    }


  return nullptr;
}


Statement const*
ExecutionFrame::
get_next_statement()
{
    if(current != end)
    {
      auto&  stmt = *current++;

      return &stmt;
    }


  return nullptr;
}


}




