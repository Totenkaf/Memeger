cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project(boost.beast)



set(SOURCES ${PROJECT_SOURCE_DIR}/src)
set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${INCLUDES})
add_subdirectory(DB)
add_subdirectory(test)
# Async HTTP Client
# add_executable(async.http.client  ${SOURCES}/async.http.client.cpp)
# target_link_libraries(async.http.client  ${Boost_LIBRARIES} -lpthread )

#  Async HTTP Server
# add_executable(async.http.server ${INCLUDES}/async.http.server.h ${SOURCES}/async.http.server.cpp ${INCLUDES}/connection.h ${SOURCES}/connection.cpp  ${INCLUDES}/request.h ${SOURCES}/request.cpp ${INCLUDES}/response.h ${SOURCES}/response.cpp)
# target_include_directories(async.http.server PUBLIC ${LIB_DATABASE_INCLUDE_DIRS})
# target_link_libraries(async.http.server  ${Boost_LIBRARIES} ${LIB_DATABASE}   -lpthread   -lpqxx -lpq)
