# Copyright 2022 by Artem Ustsov

# ==============================================================================
# I | General project settings
# ==============================================================================
project(run_db)

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Wpedantic -Werror")

add_subdirectory(project)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC database_lib)
