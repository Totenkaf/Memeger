cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
project(Memeger)

 
add_subdirectory(database)
add_subdirectory(server)
 

add_executable(run_server ServerMain.cpp)
target_link_libraries(run_server PUBLIC database_lib server_lib)

 
