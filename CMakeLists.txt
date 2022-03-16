cmake_minimum_required(VERSION 3.15)

set(VCPKG_TARGET_TRIPLET "x64-linux")

if(DEFINED ENV{HOME} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(thrift_demo CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_path(BRPC_INCLUDE_DIR brpc/)
include_directories(${BRPC_INCLUDE_DIR})
find_library(BRPC_LIBRARY libbrpc.a)
link_libraries(${BRPC_LIBRARY})

find_library(CRYPTO_LIBRARY libcrypto.a)
find_library(SSL_LIBRARY libssl.a)
find_library(THRIFT_LIBRARY libthrift.a)

find_package(glog CONFIG REQUIRED)
find_package(Thrift CONFIG REQUIRED)
find_package(leveldb CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

find_program(THRIFT_COMPILER thrift)
if (THRIFT_COMPILER)
    exec_program(${THRIFT_COMPILER}
        ARGS -version OUTPUT_VARIABLE __thrift_OUT RETURN_VALUE THRIFT_RETURN)
    string(REGEX MATCH "[0-9]+.[0-9]+.[0-9]+-[a-z]+$" THRIFT_VERSION_STRING ${__thrift_OUT})

    function(thrift_gen_cpp thrift_file THRIFT_CPP_FILES_LIST THRIFT_GEN_INCLUDE_DIR)
        set(_res)
        set(_res_inc_path)
        if(EXISTS ${thrift_file})
            get_filename_component(_target_dir ${thrift_file} NAME_WE)
            message("thrift_gen_cpp: ${thrift_file}")

            if(NOT EXISTS ${CMAKE_BINARY_DIR}/${_target_dir})
                file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${_target_dir})
            endif()
            exec_program(${THRIFT_COMPILER}
                ARGS -o "${CMAKE_BINARY_DIR}/${_target_dir}" --gen cpp ${thrift_file}
                OUTPUT_VARIABLE __thrift_OUT
                RETURN_VALUE THRIFT_RETURN)
            file(GLOB_RECURSE __result_src "${CMAKE_BINARY_DIR}/${_target_dir}/*.cpp")
            file(GLOB_RECURSE __result_hdr "${CMAKE_BINARY_DIR}/${_target_dir}/*.h")
            list(APPEND _res ${__result_src})
            list(APPEND _res ${__result_hdr})
            if(__result_hdr)
                list(GET __result_hdr 0 _res_inc_path)
                get_filename_component(_res_inc_path ${_res_inc_path} DIRECTORY)
            endif()
        else()
            message("thrift_gen_cpp: file ${thrift_file} does not exists")
        endif()
        set(${THRIFT_CPP_FILES_LIST} "${_res}" PARENT_SCOPE)
        set(${THRIFT_GEN_INCLUDE_DIR} "${_res_inc_path}" PARENT_SCOPE)
    endfunction()
endif ()
thrift_gen_cpp(
    ${CMAKE_CURRENT_SOURCE_DIR}/arbiter/prediction.thrift
    THRIFT_CPP_FILES THRIFT_TARGET_DIR)
add_library(thrift_idl STATIC ${THRIFT_CPP_FILES})
target_include_directories(thrift_idl PUBLIC ${THRIFT_TARGET_DIR})
include_directories(${THRIFT_TARGET_DIR})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCES})

add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/executables/server.cpp)
target_link_libraries(server
    PRIVATE
        -ldl
        -lpthread
        ${SSL_LIBRARY}
        ${CRYPTO_LIBRARY}
        thrift::thrift
        glog::glog
        leveldb::leveldb
        protobuf::libprotobuf
        ${BRPC_LIBRARY}
        thrift_idl
        ${PROJECT_NAME}
)

add_executable(client ${CMAKE_CURRENT_SOURCE_DIR}/executables/client.cpp)
target_link_libraries(client
    PRIVATE
        -ldl
        -lpthread
        ${SSL_LIBRARY}
        ${CRYPTO_LIBRARY}
        thrift::thrift
        glog::glog
        leveldb::leveldb
        protobuf::libprotobuf
        ${BRPC_LIBRARY}
        thrift_idl
        ${PROJECT_NAME}
)