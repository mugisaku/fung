#include"fung_space.hpp"
#include"fung_function.hpp"




namespace fung{



Space::
Space(Space&  parent_):
global(parent_.global),
parent(&parent_)
{
}




}




