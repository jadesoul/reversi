cmake_minimum_required(VERSION 2.6)
project (reversi)

set(CMAKE_BUILD_TYPE Release)

set(PREFIX ${CMAKE_CURRENT_LIST_DIR}/..)
set(BINDIR "${PREFIX}/bin")
set(INCDIR "${PREFIX}/include")

set(CMAKE_CXX_FLAGS "-fPIC -Wall -D_GNU_SOURCE -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -DNDEBUG")

include_directories(${INCDIR})

add_executable(reversi reversi.cpp)
install(TARGETS reversi DESTINATION ${BINDIR})
