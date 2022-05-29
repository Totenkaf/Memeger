# set the minimum required version of cmake needed by this CMake project.
# this command is needed to set version and policy
# settings before invoking other commands
project(ui)
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -pthread")

# set the name of the project, the version and which languages
# are needed to build this project
# this command must immediately follow the cmake_minimum_required command

# GTK 3 dependency
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# some test warning levels for GCC
set(WARNING_LEVELS_GCC
    -Werror
)
set(WARNING_LEVELS_GCC_DEBUG
    -Wfloat-equal
    -Wextra
    -Wall
    -Wundef
    -Wshadow
    -Wpointer-arith
    -Wcast-align
    -Wstrict-overflow=5
    -Wwrite-strings
    #-Waggregate-return
    -Wcast-qual
    -Wswitch-default
    -Wswitch-enum
    -Wconversion
)    

set(${PROJECT_NAME}_lib_includes ${PROJECT_SOURCE_DIR}/include)
message("UI_INCLUDES_DIR = ${${PROJECT_NAME}_lib_includes}")

set(${PROJECT_NAME}_lib_sources ${PROJECT_SOURCE_DIR}/src)
message("UI_SOURCE_DIR = ${${PROJECT_NAME}_lib_sources}\n")

set(UI_LIB_FILES ${${PROJECT_NAME}_lib_sources}/chat.cpp
                 ${${PROJECT_NAME}_lib_sources}/search.cpp
                 ${${PROJECT_NAME}_lib_sources}/login-signup.cpp
)

# include directory for third party libraries
include(ExternalProject)
include_directories(vendor/)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/theme.css ${CMAKE_BINARY_DIR}/theme.css COPYONLY)

add_library(ui_lib SHARED ${UI_LIB_FILES})
target_include_directories(ui_lib PUBLIC ${GTK3_INCLUDE_DIRS} ${${PROJECT_NAME}_lib_includes})
target_link_libraries(ui_lib PUBLIC client_lib ${GTK3_LIBRARIES})
