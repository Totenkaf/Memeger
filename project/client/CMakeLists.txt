cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project(boost.beast)



set(SOURCES ${PROJECT_SOURCE_DIR}/src)
set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${INCLUDES})
 

#  Async HTTP Client

add_library(client_lib SHARED ${INCLUDES}/async.http.client.h ${SOURCES}/async.http.client.cpp)
target_link_libraries(client_lib  database_lib ${Boost_LIBRARIES} -lpthread  )
 