cmake_minimum_required(VERSION 2.6)

foreach(app reversi board neuraltrain)
    add_executable(${app} ${app}.cpp)
    install(TARGETS ${app} DESTINATION ${BINDIR})
    target_link_libraries(${app} engine_lib)
endforeach(app)
