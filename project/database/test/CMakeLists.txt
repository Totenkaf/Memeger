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
include(CTest)
find_package(GTest REQUIRED)

include_directories(${INCLUDES})
set(db_test_src ${PROJECT_SOURCE_DIR}/postgresql_db_unit.cpp)
set(models_test_src ${PROJECT_SOURCE_DIR}/models_unit.cpp)
# set(integrity_test_src ${PROJECT_SOURCE_DIR}/postgresql_db_integrity.cpp)

# ------------------------------------------------------------------------------
# 2 | Executables
# ------------------------------------------------------------------------------
# add_executable(integrity_test ${integrity_test_src})

# target_include_directories(integrity_test PUBLIC ${INCLUDES})
# target_link_libraries(integrity_test ${MODELS_LIB_FILES} ${DATABASE_LIB_FILES})
# target_link_libraries(integrity_test ${GTEST_LIBRARIES} -pthread)

add_executable(db_test ${db_test_src})
add_executable(models_test ${models_test_src})

target_link_libraries(db_test ${DB_LIB} -lpqxx -lpq)
target_link_libraries(db_test ${GTEST_LIBRARIES} -pthread)

target_link_libraries(models_test ${DB_LIB} -lpqxx -lpq)
target_link_libraries(models_test ${GTEST_LIBRARIES} -pthread)


# ==============================================================================
# III | Check settings
# ==============================================================================
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")
endif()
