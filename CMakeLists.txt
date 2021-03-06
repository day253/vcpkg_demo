cmake_minimum_required(VERSION 3.15)

set(VCPKG_TARGET_TRIPLET "x64-linux")

if(DEFINED ENV{HOME} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(rpcpress CXX)

find_path(BRPC_INCLUDE_DIR brpc/)
include_directories(${BRPC_INCLUDE_DIR})

find_library(BRPC_LIBRARY libbrpc.a)
link_libraries(${BRPC_LIBRARY})

find_library(CRYPTO_LIBRARY libcrypto.a)
find_library(SSL_LIBRARY libssl.a)
find_library(THRIFT_LIBRARY libthrift.a)

find_package(glog CONFIG REQUIRED)
find_package(leveldb CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(rpcpress ${SOURCES})
target_compile_features(rpcpress PRIVATE cxx_std_11)

target_link_libraries(rpcpress
    PRIVATE
    -ldl
    -lpthread
    ${BRPC_LIBRARY}
    ${CRYPTO_LIBRARY}
    ${SSL_LIBRARY}
    ${THRIFT_LIBRARY}
    glog::glog
    leveldb::leveldb
    protobuf::libprotobuf
)