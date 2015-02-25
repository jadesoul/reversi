cmake_minimum_required(VERSION 2.6)

foreach(app reversi board train_neural_model gen_train_data addrfind_vs_bitop_speed)
    add_executable(${app} ${app}.cpp)
    install(TARGETS ${app} DESTINATION ${BINDIR})
    target_link_libraries(${app} engine_lib)
endforeach(app)
