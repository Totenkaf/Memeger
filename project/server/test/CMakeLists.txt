cmake_minimum_required(VERSION 3.14)

project(test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")
 

# ------------------------------------------------------------------------------
# 1 | Includes
# ------------------------------------------------------------------------------
include(CTest)
find_package(GTest REQUIRED)

set(unit ${PROJECT_SOURCE_DIR}/unit.cpp)

add_executable(unit_test ${unit})
target_link_libraries(unit_test server_lib ${Boost_LIBRARIES} database_lib -lpthread)
target_link_libraries(unit_test ${GTEST_LIBRARIES} -lpthread)

 

 