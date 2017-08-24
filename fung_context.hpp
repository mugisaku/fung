#ifndef FUNG_Context_HPP_INCLUDED
#define FUNG_Context_HPP_INCLUDED


#include"fung_value.hpp"
#include"fung_error.hpp"
#include"fung_ExecutionFrame.hpp"
#include<vector>
#include<memory>


namespace fung{


class GlobalSpace;


class
Context
{
  GlobalSpace const*  global_space;

  std::vector<ExecutionFrame>  frame_stack;

  Value  returned_value;

  bool  interruption_flag=false;

public:
  Context(std::unique_ptr<GlobalSpace> const&  gsp): global_space(gsp.get()){}

  std::vector<ExecutionFrame> const*  operator->() const{return &frame_stack;}

  bool  prepare_to_run(std::string const&  function_name);
  bool  run();

  void  enter(Function const&  fn);
  void  leave();

  void  entry(std::string const&  name, Expression const&  expr);
  void  entry(std::string const&  name, Value const&  val);

  void  hold_returned_value(Value  value){returned_value = value;}

  bool  test_interruption_flag() const{return interruption_flag;}
  void   set_interruption_flag(){interruption_flag = true;}

  Value const&  get_returned_value() const{return returned_value;}

  Value  find_value(std::string const&  name) const;

  Value  operator[](std::string const&  name) const;

};




}




#endif




