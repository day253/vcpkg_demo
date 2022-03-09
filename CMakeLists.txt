cmake_minimum_required(VERSION 3.15)

set(VCPKG_TARGET_TRIPLET "x64-linux")
set(CMAKE_TOOLCHAIN_FILE "/workspaces/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(rpcpress CXX)

find_path(BRPC_INCLUDE_DIR brpc/)
find_library(BRPC_LIBRARY libbrpc.a)

include_directories(${BRPC_INCLUDE_DIR})
link_libraries(${BRPC_LIBRARY})

find_package(protobuf CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(leveldb CONFIG REQUIRED)

find_library(THRIFT_LIBRARY libthrift.a)
find_library(SSL_LIBRARY libssl.a)
find_library(CRYPTO_LIBRARY libcrypto.a)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(rpcpress ${SOURCES})
target_compile_features(rpcpress PRIVATE cxx_std_11)

target_link_libraries(rpcpress
    PRIVATE
    -lpthread
    ${SSL_LIBRARY}
    ${CRYPTO_LIBRARY}
    -ldl
    ${BRPC_LIBRARY}
    glog::glog
    leveldb::leveldb
    ${THRIFT_LIBRARY}
    protobuf::libprotobuf
)