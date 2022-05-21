# Copyright 2022 by Artem Ustsov

# ==============================================================================
# I | General project settings
# ==============================================================================

cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 20)
# ==============================================================================
# II | Preinstall settings
# ==============================================================================

# ------------------------------------------------------------------------------
# 1 | Includes
# ------------------------------------------------------------------------------
find_package(GTest REQUIRED)

include_directories(${INCLUDES})
set(db_test_src ${PROJECT_SOURCE_DIR}/postgresql_db_unit.cpp)


# ------------------------------------------------------------------------------
# 2 | Executables
# ------------------------------------------------------------------------------4
add_executable(db_test ${db_test_src})
target_link_libraries(db_test database_lib ${GTEST_LIBRARIES} -pthread)


# ==============================================================================
# III | Check settings
# ==============================================================================
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")
endif()
