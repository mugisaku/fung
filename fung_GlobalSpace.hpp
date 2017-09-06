#ifndef FUNG_GlobalSpace_HPP_INCLUDED
#define FUNG_GlobalSpace_HPP_INCLUDED


#include<vector>
#include<memory>




namespace fung{


class Function;
class FunctionBody;
class Variable;


class
GlobalSpace
{
  std::vector<std::unique_ptr<Function>>  function_list;
  std::vector<std::unique_ptr<Variable>>  variable_list;
  std::vector<std::unique_ptr<FunctionBody>>  body_list;

public:
  GlobalSpace();

  void  append_function(std::string&&  name, Function*  fn);
  void  append_variable(Variable*  var);
  void  append_function_body(FunctionBody*  fnbody);

  std::unique_ptr<Variable> const&  find(std::string const&  name) const;

  void  print() const;

};




}




#endif




