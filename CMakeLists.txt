cmake_minimum_required(VERSION 3.15)

include(/root/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(rpcpress CXX)

find_package(brpc REQUIRED)
find_package(leveldb CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(rpcpress ${SOURCES})
target_compile_features(rpcpress PRIVATE cxx_std_11)

target_link_libraries(rpcpress
    PRIVATE
    brpc::brpc
    leveldb::leveldb
    gflags::gflags
    protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
)