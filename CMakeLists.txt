cmake_minimum_required(VERSION 3.15)

set(VCPKG_TARGET_TRIPLET "x64-linux")
set(CMAKE_TOOLCHAIN_FILE "/home/daijunkai/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(rpcpress CXX)

find_path(BRPC_INCLUDE_DIR brpc/)
find_library(BRPC_LIBRARY brpc)
include_directories(${BRPC_INCLUDE_DIR})
link_libraries(${BRPC_LIBRARY})

find_package(protobuf CONFIG REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(rpcpress ${SOURCES})
target_compile_features(rpcpress PRIVATE cxx_std_11)

target_link_libraries(rpcpress
    PRIVATE
    -lpthread
    ${BRPC_LIBRARY}
    protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
)