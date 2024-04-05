cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME invlib)

add_compile_definitions("CANLIB_PARKING=")
add_compile_definitions(CANLIB_TIMESTAMP)

add_library(invlib STATIC inverter.c inverter_conversions.c)
