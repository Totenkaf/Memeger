cmake_minimum_required(VERSION 3.8)
project(run_ui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -pthread -Wall -Wextra -Wpedantic -Werror")

add_subdirectory(project/ui)
add_executable(${PROJECT_NAME} project/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ui_lib)