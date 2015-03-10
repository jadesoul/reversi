cmake_minimum_required(VERSION 2.6)

aux_source_directory(. SRCS)
add_executable(reversi ${SRCS})
install(TARGETS reversi DESTINATION ${BINDIR})