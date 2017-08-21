#ifndef FUNG_GlobalSpace_HPP_INCLUDED
#define FUNG_GlobalSpace_HPP_INCLUDED


#include"fung_space.hpp"
#include<vector>
#include<memory>




namespace fung{


class Function;


class
GlobalSpace: public Space
{
  std::vector<std::unique_ptr<Function>>  function_list;

public:
  GlobalSpace(): Space(*this){}

  Function const&  append_function(Function*  fn);

  Function const*  find_function(std::string const&  name) const;

  void  print() const;

};




}




#endif




