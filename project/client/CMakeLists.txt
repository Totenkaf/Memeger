
project(boost_client)

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -pthread")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
# set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")

find_package(Boost)
set(${PROJECT_NAME}_lib_includes ${PROJECT_SOURCE_DIR}/include)
message("CLIENT_INCLUDES_DIR = ${${PROJECT_NAME}_lib_includes}")

set(${PROJECT_NAME}_lib_sources ${PROJECT_SOURCE_DIR}/src)
message("CLIENT_SOURCE_DIR = ${${PROJECT_NAME}_lib_sources}\n")

set(CLIENT_LIB_FILES ${${PROJECT_NAME}_lib_sources}/async.http.client.cpp)

#  Async HTTP Client
add_library(client_lib SHARED ${CLIENT_LIB_FILES})
target_include_directories(client_lib PUBLIC ${BOOST_INCLUDE_DIRS} ${${PROJECT_NAME}_lib_includes})
target_link_libraries(client_lib PUBLIC database_lib ${BOOST_LIBRARIES} -lpthread)
