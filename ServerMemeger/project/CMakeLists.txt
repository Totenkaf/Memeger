# Copyright 2022 by Artem Ustsov

# ==============================================================================
# I | General project settings
# ==============================================================================
cmake_minimum_required(VERSION 3.8)
set(project_name database)
project(${project_name})


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Wpedantic -Werror")

# ==============================================================================
# II | Preinstall settings
# ==============================================================================

# ------------------------------------------------------------------------------
# 1 | Include subdirectories
# ------------------------------------------------------------------------------

set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include(ExternalProject)

# find_package(libpqxx REQUIRED)
# message(STATUS ${PostgreSQL_INCLUDE_DIRS})

include_directories(${INCLUDES})
message("INCLUDES_DIR = ${INCLUDES}")

# ------------------------------------------------------------------------------
# 2 | Add libraries
# ------------------------------------------------------------------------------
set(SOURCES ${PROJECT_SOURCE_DIR}/src)
message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

set(MODELS_LIB_FILES ${SOURCES}/models.cpp)
message("MODELS_LIB_FILES = ${MODELS_LIB_FILES}")


set(DATABASE_LIB_FILES ${SOURCES}/postgresql_db.cpp)
message("DATABASE_LIB_FILES = ${DATABASE_LIB_FILES}")

add_library(database_lib SHARED ${MODELS_LIB_FILES} ${DATABASE_LIB_FILES})

set(LIB_DATABASE_INCLUDE_DIRS ${INCLUDES} PARENT_SCOPE)
set(LIB_DATABASE database_lib PARENT_SCOPE)
 

# ------------------------------------------------------------------------------
# 2 | Executable Main
# ------------------------------------------------------------------------------
# add_executable(${PROJECT_NAME} ${SOURCES}/main.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE database_lib -lpqxx -lpq)

# ==============================================================================
# IV | Tests
# ==============================================================================

# ------------------------------------------------------------------------------
# 1 | Integrity tests
# ------------------------------------------------------------------------------
option(BUILD_TESTS "Build all tests." OFF) # Makes boolean 'unit_test' available.

if(BUILD_TESTS)
  add_subdirectory(project/test)
endif()