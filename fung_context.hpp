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
  std::unique_ptr<GlobalSpace> const&  global_space;

  std::vector<ExecutionFrame>  frame_stack;

  bool  interruption_flag=false;

  Value  start();
  void   leave();

  void  check_depth() const;

public:
  Context(std::unique_ptr<GlobalSpace> const&  gsp): global_space(gsp){}

  std::vector<ExecutionFrame> const*  operator->() const{return &frame_stack;}

  Value  run(std::string const&  function_name, List&&  args);

  Value  call(std::string const&  fn_name, Function const&  fn, List&&  args);
  Value  call(StatementList const&  stmtls);

  void  entry(std::string const&  name, Expression const&  expr);
  void  entry(std::string const&  name, Value const&  val);

  void  entry(ParameterList const&  parals, List const&  ls);

  bool  test_interruption_flag() const{return interruption_flag;}
  void   set_interruption_flag(){interruption_flag = true;}

  Value  find_value(std::string const&  name) const;

  Value  operator[](std::string const&  name) const;

};




}




#endif




