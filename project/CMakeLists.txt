cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
project(Memeger)

add_subdirectory(client)
add_subdirectory(database)
add_subdirectory(server)
add_subdirectory(ui)

add_executable(run_server ServerMain.cpp)
target_link_libraries(run_server PUBLIC database_lib server_lib)

add_executable(run_client ClientMain.cpp)
target_link_libraries(run_client PUBLIC ui_lib client_lib)
