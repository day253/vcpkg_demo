cmake_minimum_required(VERSION 3.15)

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

message("VCPKG_TARGET_TRIPLET=${VCPKG_TARGET_TRIPLET}")

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

project(rpcpress CXX)

find_package(brpc CONFIG REQUIRED)
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