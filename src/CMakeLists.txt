cmake_minimum_required(VERSION 2.6)

aux_source_directory(pypp PYPP_SRCS)
aux_source_directory(engine ENGINE_SRCS)
aux_source_directory(engine/ai ENGINE_AI_SRCS)
set(SOURCES ${PYPP_SRCS} ${ENGINE_SRCS} ${ENGINE_AI_SRCS})

add_library(engine_lib STATIC ${SOURCES})
set_target_properties(engine_lib PROPERTIES OUTPUT_NAME "engine")
install(TARGETS engine_lib DESTINATION ${LIBDIR})

add_library(engine_so SHARED ${SOURCES})
target_link_libraries(engine_so engine_lib dl pthread)
set_target_properties(engine_so PROPERTIES OUTPUT_NAME "engine")
set_target_properties(engine_so PROPERTIES VERSION 1.2 SOVERSION 1)
install(TARGETS engine_so DESTINATION ${LIBDIR})
