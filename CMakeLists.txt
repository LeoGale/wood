cmake_minimum_required(VERSION 2.6)

project (wood C CXX)

enable_testing()


set(WOOD_VERSION_MAJOR 0)
set(WOOD_VERSION_MINOR 1)

IF(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "release")
ENDIF()

set(CXX_FLAGS
    -Wno-unused-parameter
    -Wno-old-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -std=c++0x
)

IF(CMAKE_BUILD_BITS EQUAL 32)
    list(APPEND CXX_FLAG "-m32")
ENDIF()

IF(CMAKE_BUILD_TYPE EQUAL "debug")
    list(APPEND CXX_FLAGS "-g")
ELSE()
    list(APPEND CXX_FLAGS "-o2")
ENDIF()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-o0")
set(CMAKE_CXX_FLAGS_RELEASE "-o2 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

message(STATUS "CXX DEBUG FLAGS ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX RELEASE FLAGS ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CXX FLAGS ${CMAKE_CXX_FLAGS}")
message(STATUS "BUILD EXAMPLES ${CMAKE_BUILD_EXAMPLES}")
message(STATUS "EXCUTABLE OUTPUT PATH ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY OUTPUT PATH ${LIBRARY_OUTPUT_PATH}")
message(STATUS "PROJECT SOURCE DIR ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT BINARY DIR ${PROJECT_BINARY_DIR}")

configure_file(
    ${PROJECT_SOURCE_DIR}/Config.h
    @ONLY
)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(wood/base)
add_subdirectory(wood/net)

IF(NOT CMAKE_BUILD_NO_EXAMPLES)
    add_subdirectory(examples)
ELSE()
    message(STATUS "To build project without examples!")
ENDIF()

