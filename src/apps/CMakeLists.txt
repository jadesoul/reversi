cmake_minimum_required(VERSION 2.6)

foreach(app reversi board)
    add_executable(${app} reversi.cpp)
    install(TARGETS ${app} DESTINATION ${BINDIR})
    target_link_libraries(${app} engine_lib)
endforeach(app)
