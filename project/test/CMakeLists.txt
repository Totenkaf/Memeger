# Copyright 2022 by Artem Ustsov

# ==============================================================================
# I | General project settings
# ==============================================================================

cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 11)
# ==============================================================================
# II | Preinstall settings
# ==============================================================================

# ------------------------------------------------------------------------------
# 1 | Includes
# ------------------------------------------------------------------------------
include(CTest)
find_package(GTest REQUIRED)
set(unit_test_src ${PROJECT_SOURCE_DIR}/postgresql_db_unit.cpp)
set(integrity_test_src ${PROJECT_SOURCE_DIR}/postgresql_db_integrity.cpp)

# ------------------------------------------------------------------------------
# 2 | Executables
# ------------------------------------------------------------------------------
add_executable(unit_test ${unit_test_src})
add_executable(integrity_test ${integrity_test_src})

target_include_directories(input_test PUBLIC ${LIB_INCLUDE_DIRS} ${UTILS_INCLUDE_DIRS})
target_include_directories(input_test PUBLIC ${LIB_INCLUDE_DIRS} ${UTILS_INCLUDE_DIRS})

target_link_libraries(parallel_test ${LIB_PARALLEL_LIBRARIES} ${UTILS_LIBRARIES})
target_link_libraries(parallel_test ${LIB_PARALLEL_LIBRARIES} ${UTILS_LIBRARIES})

target_link_libraries(parallel_test ${GTEST_LIBRARIES} -pthread)
target_link_libraries(parallel_test ${GTEST_LIBRARIES} -pthread)

# ==============================================================================
# III | Check settings
# ==============================================================================
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")
endif()
