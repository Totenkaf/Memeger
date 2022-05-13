cmake_minimum_required(VERSION 3.8)
project(ui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Wpedantic -Werror")

set(${project_name}_lib_includes ${PROJECT_SOURCE_DIR}/include)
message("UI_INCLUDES_DIR = ${${project_name}_lib_includes}")
include(ExternalProject)

set(${project_name}_lib_sources ${PROJECT_SOURCE_DIR}/src)
message("UI_SOURCE_DIR = ${${project_name}_lib_sources}")

set(CHAT_LIB_FILES ${${project_name}_lib_sources}/chat.cpp)
message("CHAT_LIB_FILES = ${CHAT_LIB_FILES}")


set(SEARCH_LIB_FILES ${${project_name}_lib_sources}/search.cpp)
message("SEARCH_LIB_FILES = ${SEARCH_LIB_FILES}")

add_library(ui_lib SHARED ${CHAT_LIB_FILES} ${SEARCH_LIB_FILES})
target_include_directories(ui_lib PUBLIC ${${project_name}_lib_includes})
target_link_libraries(ui_lib PUBLIC -lpqxx -lpq)

option(BUILD_TESTS "Build all tests." OFF) # Makes boolean 'unit_test' available.

if(BUILD_TESTS)
  add_subdirectory(test)
endif()