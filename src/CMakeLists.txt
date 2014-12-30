cmake_minimum_required(VERSION 2.6)
project (reversi)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INSTALL_PREFIX ~)
set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(CGI_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cgi-bin")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")

set(CMAKE_CXX_FLAGS "-fPIC -Wall -D_GNU_SOURCE -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -DNDEBUG")

include_directories(${INC_INSTALL_DIR})
link_directories(${LIB_INSTALL_DIR})

set(SRCS logging.cpp random.cpp types.cpp)

set(app reversi)
add_executable(${app} ${app}.cpp ${SRCS})
install(TARGETS ${app} DESTINATION ${BIN_INSTALL_DIR})
target_link_libraries(${app} soul)

set(app engine)
add_executable(${app} ${app}.cpp ${SRCS})
install(TARGETS ${app} DESTINATION ${CGI_INSTALL_DIR})
target_link_libraries(${app} libsoul.a)
