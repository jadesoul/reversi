cmake_minimum_required(VERSION 2.6)
project (reversi)

set(CMAKE_BUILD_TYPE Release)

set(PREFIX ${CMAKE_CURRENT_LIST_DIR}/..)
set(BINDIR "${PREFIX}/bin")
set(CGIDIR "${PREFIX}/cgi-bin")
set(LIBDIR "${PREFIX}/lib")
set(INCDIR "${PREFIX}/include")

set(CMAKE_CXX_FLAGS "-fPIC -Wall -D_GNU_SOURCE -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -DNDEBUG")

include_directories(${INCDIR})
include_directories(/usr/include/python2.7)
#link_directories(${LIBDIR})

set(SRCS logging.cpp random.cpp types.cpp)

add_executable(reversi reversi.cpp ${SRCS})
install(TARGETS reversi DESTINATION ${BINDIR})

add_executable(engine engine.cpp ${SRCS})
install(TARGETS engine DESTINATION ${CGIDIR})

add_executable(server server.cpp)
install(TARGETS server DESTINATION ${BINDIR})
target_link_libraries(server boost_system)

add_library(_reversi SHARED _reversi.cpp)
install(TARGETS _reversi DESTINATION ${BINDIR})
#target_link_libraries(_reversi boost_python)

