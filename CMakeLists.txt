cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "protobuf_test")

find_package(Protobuf REQUIRED)

add_library(invlib STATIC inverter.c inverter.pb.cc inverter_proto.cpp)
target_link_libraries(invlib ${Protobuf_LIBRARIES})

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES} invlib)