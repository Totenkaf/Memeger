cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)
project(Memeger)
add_subdirectory(database)
add_subdirectory(server)
add_subdirectory(client)
add_executable(run_server ServerMain.cpp)
target_link_libraries(run_server PUBLIC database_lib server_lib)

add_executable(run_client ClientMain.cpp)
target_link_libraries(run_client PUBLIC  client_lib)
 
 