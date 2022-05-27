
project(boost_server)

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -pthread")


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -lgcov")


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

 

# ------------------------------------------------------------------------------
# 2 | Clang-Format
# ------------------------------------------------------------------------------

if(ENABLE_CLANG_FORMAT)
  file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
         ${PROJECT_SOURCE_DIR}/src/*.cpp
         ${PROJECT_SOURCE_DIR}/include/*.h
         ${PROJECT_SOURCE_DIR}/test/*.cpp
  )
  add_custom_target(
    clang-format-server
    COMMAND /usr/bin/clang-format
    -i
    -style=LLVM
    ${ALL_CXX_SOURCE_FILES}
  )
endif()

option(BUILD_TESTS "Build all tests." OFF)
if(BUILD_TESTS)
  add_subdirectory(test)
endif()

option(ENABLE_CPPCHECK "Build all tests." OFF) # Makes boolean 'test' available.
if(ENABLE_CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
    )

    ExternalProject_Add(
        cppcheck
        GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
        GIT_TAG             1.79
        GIT_SHALLOW         1
        CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
        PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
        TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
        STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
        DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
        SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
        BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
        --enable=all
        --suppress=missingIncludeSystem
        --std=c11
        --verbose
        --error-exitcode=1
        --language=c
        --check-config
        -DMAIN=main
        -I ${CMAKE_SOURCE_DIR}/project/server
           ${CMAKE_SOURCE_DIR}/project/server/src/*.cpp
           ${CMAKE_SOURCE_DIR}/project/server/include/*.h
           ${CMAKE_SOURCE_DIR}/project/server/test/*.cpp
    )

    add_custom_target(
        check-server
        COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
        COMMENT "running cppcheck"
    )
endif()

if(ENABLE_CLANG_TIDY)

    find_program(CLANG_TIDY_BIN clang-tidy-10)
    find_program(RUN_CLANG_TIDY_BIN run-clang-tidy-10.py)

    if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy-10")
    endif()

    if(RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate run-clang-tidy-10.py")
    endif()

    list(APPEND RUN_CLANG_TIDY_BIN_ARGS
        -clang-tidy-binary ${CLANG_TIDY_BIN}
        -header-filter=.*
        -checks=perf*,mode*
    )
    add_custom_target(
        tidy
        COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
        COMMENT "running clang tidy"
    )
endif()


if(ENABLE_ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif()

if(ENABLE_USAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

if(ENABLE_TSAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()