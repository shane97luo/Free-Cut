
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
        message(FATAL_ERROR "GCC ≥10 或 Clang ≥10 需要支持 C++20")
    endif()

    # 显式指定标准（部分旧版本需要）
    add_compile_options(-std=c++20)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target/lib)

## set test unit to build/units
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/target/bin)