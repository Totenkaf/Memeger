cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project(boost.beast)



 

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${INCLUDES})
add_subdirectory(project)

# Async HTTP Client
add_executable(async.http.client  async.http.client.cpp)
target_link_libraries(async.http.client  ${Boost_LIBRARIES} -lpthread )

#  Async HTTP Server
add_executable(async.http.server async.http.server.h async.http.server.cpp connection.h connection.cpp )
target_include_directories(async.http.server PUBLIC ${LIB_DATABASE_INCLUDE_DIRS})
target_link_libraries(async.http.server  ${Boost_LIBRARIES} ${LIB_DATABASE}   -lpthread   -lpqxx -lpq)
