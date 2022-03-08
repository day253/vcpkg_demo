cmake_minimum_required(VERSION 3.15)

project(rpcpress CXX)

find_package(brpc CONFIG REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(rpcpress ${SOURCES})
target_compile_features(rpcpress PRIVATE cxx_std_11)

target_link_libraries(rpcpress PRIVATE brpc::brpc)