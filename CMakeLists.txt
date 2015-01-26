cmake_minimum_required(VERSION 2.6)
project (reversi)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(PREFIX ${CMAKE_CURRENT_LIST_DIR})
set(BINDIR "${PREFIX}/bin")
set(LIBDIR "${PREFIX}/lib")
set(SRCDIR "${PREFIX}/src")
set(INCDIR "${PREFIX}/include")

#set(CMAKE_CXX_FLAGS "-fPIC -Wall -D_GNU_SOURCE -std=c++0x")
set(CMAKE_CXX_FLAGS "-fPIC -w -D_GNU_SOURCE -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -DNDEBUG")

include_directories(${INCDIR} ${SRCDIR})

add_subdirectory(src)