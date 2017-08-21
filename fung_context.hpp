#ifndef FUNG_Context_HPP_INCLUDED
#define FUNG_Context_HPP_INCLUDED


#include"fung_value.hpp"
#include<vector>


namespace fung{


class GlobalSpace;


struct
Variable
{
  std::string  name;

  Value  value;

  Variable(std::string const&  name_, Value&&  v): name(name_), value(v){}

};


struct
Frame
{
  Function const&  function;

  std::vector<Variable>  variable_list;

  Frame(Function const&  fn): function(fn){}

  void  append(std::string const&  name, Value  value)
  {
    variable_list.emplace_back(name,std::move(value));
  }

};


class
Context
{
  GlobalSpace const*  global_space;

  std::vector<Frame>  frame_stack;

public:
  Context(GlobalSpace const*  gsp=nullptr): global_space(gsp){}

  std::vector<Frame> const*  operator->() const{return &frame_stack;}

  void  push_frame(Frame&&  frame);
  void  pop_frame();

  Value  find_value(std::string const&  name) const;

  Value  operator[](std::string const&  name) const;

};




}




#endif




