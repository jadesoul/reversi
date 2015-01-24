cmake_minimum_required(VERSION 2.6)

add_executable(reversi reversi.cpp)
install(TARGETS reversi DESTINATION ${BINDIR})
target_link_libraries(reversi engine_so)
