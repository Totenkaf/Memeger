cmake_minimum_required(VERSION 3.14)

project(test)
set(CMAKE_CXX_STANDARD 20)
 

# ------------------------------------------------------------------------------
# 1 | Includes
# ------------------------------------------------------------------------------
include(CTest)
find_package(GTest REQUIRED)

include_directories(${INCLUDES})
set(unit ${PROJECT_SOURCE_DIR}/unit.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")
 
add_executable(unit_test ${unit})
#target_include_directories(async.http.server PUBLIC ${LIB_DATABASE_INCLUDE_DIRS})
target_link_libraries(unit_test   ${SOURCES}/async.http.server.cpp  ${SOURCES}/connection.cpp   ${SOURCES}/request.cpp  ${SOURCES}/response.cpp)

target_link_libraries(unit_test ${Boost_LIBRARIES} ${LIB_DATABASE}   -lpthread   -lpqxx -lpq)

 
target_link_libraries(unit_test ${GTEST_LIBRARIES} -pthread)

 

# if(ENABLE_COVERAGE)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")
# endif()