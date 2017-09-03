cmake_minimum_required(VERSION 2.8)


if(DEFINED SLP)
else()
  set(SLP "0")
endif()


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "  sleep ${SLP};  CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  "/usr/local/bin/gcc")
set(CMAKE_C_FLAGS   "-std=c14 -Werror")
set(CMAKE_CXX_COMPILER  "/usr/local/bin/g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Werror")
set(CMAKE_CXX_LINK_FLAGS  "-lstdc++")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")


if(EM)
  set(EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-std=c++14 -Werror -Wno-switch -O2")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1")
endif()




set(SOURCES
  fung_Expression.cpp
  fung_ExpressionNode.cpp
  fung_ExpressionNode__evaluate.cpp
  fung_ExpressionMaker.cpp
  fung_statement.cpp
  fung_function.cpp
  fung_parameter.cpp
  fung_value.cpp
  fung_value__get_property.cpp
  fung_value__operate.cpp
  fung_value__convert.cpp
  fung_space.cpp
  fung_context.cpp
  fung_variable.cpp
  fung_GlobalSpace.cpp
  fung_ExecutionFrame.cpp
  fung_cursor.cpp
  fung_pointer.cpp
  fung__skip.cpp
  fung_unicode.cpp
  fung_token.cpp
  fung__read_token.cpp
  fung__read_list.cpp
  fung__make_global_space.cpp
)


include_directories(.)
include_directories(./preprocessing)
add_executable("fung${EXTENSION}" ${SOURCES} fung.cpp)
add_executable("testBasicString${EXTENSION}" testBasicString.cpp)
add_executable("testBranch${EXTENSION}" testBranch.cpp)
add_executable("testBranchBud${EXTENSION}" testBranchBud.cpp)




