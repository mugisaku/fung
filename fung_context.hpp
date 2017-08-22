#ifndef FUNG_Context_HPP_INCLUDED
#define FUNG_Context_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_error.hpp"
#include<vector>
#include<memory>


namespace fung{


class GlobalSpace;
class Expression;
class Function;


struct
Variable
{
  std::string  name;

  Value  value;

  Variable(std::string const&  name_, Value const&  v): name(name_), value(v){}

};


struct
Frame
{
  Function const&  function;

  std::vector<Variable>  variable_list;

  Frame(Function const&  fn): function(fn){}

  void  append(Variable&&  var)
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

};


class
Context
{
  GlobalSpace const*  global_space;

  std::vector<Frame>  frame_stack;

  Value  returned_value;

public:
  Context(std::unique_ptr<GlobalSpace> const&  gsp): global_space(gsp.get()){}

  std::vector<Frame> const*  operator->() const{return &frame_stack;}

  void  enter(Function const&  fn);
  void  leave();

  void  entry(std::string const&  name, Expression const&  expr);
  void  entry(std::string const&  name, Value const&  val);

  void  hold_returned_value(Value  value){returned_value = value;}

  Value const&  get_returned_value() const{return returned_value;}

  Value  find_value(std::string const&  name) const;

  Value  operator[](std::string const&  name) const;

};




}




#endif




