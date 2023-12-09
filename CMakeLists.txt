cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME invlib)

add_compile_definitions(CANLIB_PARKING)
add_compile_definitions(CANLIB_TIMESTAMP)

add_library(invlib STATIC inverter.c)
target_link_libraries(invlib ${Protobuf_LIBRARIES})