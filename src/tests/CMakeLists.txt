
include(FetchContent)

# googletest
# FetchContent_Declare(
#     googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG v1.16.0
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest  # 指定源文件目录
#     # BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/googletest/build  # 指定构建目录
# )

# FetchContent_MakeAvailable(googletest)

add_subdirectory(units)

