cmake_minimum_required(VERSION 3.12) # version can be different

project(my_cpp_project) #name of your project

add_subdirectory(googletest) # add googletest subdirectory

include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

add_executable(mytests mytests.cpp LPTest.cpp Chained8Test.cpp QPTest.cpp RHTest.cpp Chained8Test.cpp Chained24Test.cpp CuckooTest.cpp) # add this executable
# QPTest.cpp RHTest.cpp Chained8Test.cpp Chained24Test.cpp
target_link_libraries(mytests PRIVATE gtest) # link google test to this executable








