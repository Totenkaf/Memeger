cmake_minimum_required(VERSION 3.8)
project(ui)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Wpedantic")

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
target_link_libraries(ui_lib ${GTK3_LIBRARIES})

option(BUILD_TESTS "Build all tests." OFF) # Makes boolean 'unit_test' available.

if(BUILD_TESTS)
  add_subdirectory(test)
endif()