
project(boost_server)

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -pthread")


# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")


find_package(Boost)
set(${PROJECT_NAME}_lib_includes ${PROJECT_SOURCE_DIR}/include)
message("SERVER_INCLUDES_DIR = ${${PROJECT_NAME}_lib_includes}")

set(${PROJECT_NAME}_lib_sources ${PROJECT_SOURCE_DIR}/src)
message("SERVER_SOURCE_DIR = ${${PROJECT_NAME}_lib_sources}\n")

set(SERVER_LIB_FILES ${${PROJECT_NAME}_lib_sources}/async.http.server.cpp
                     ${${PROJECT_NAME}_lib_sources}/connection.cpp
                     ${${PROJECT_NAME}_lib_sources}/request.cpp
                     ${${PROJECT_NAME}_lib_sources}/response.cpp
)

add_library(server_lib SHARED ${SERVER_LIB_FILES})
target_include_directories(server_lib PUBLIC ${BOOST_INCLUDE_DIRS} ${${PROJECT_NAME}_lib_includes})
target_link_libraries(server_lib PUBLIC database_lib ${BOOST_LIBRARIES} -lpthread)
