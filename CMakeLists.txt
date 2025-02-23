cmake_minimum_required(VERSION 3.20)


project(FreeCutStudio)


include(cmake/build.cmake)

add_subdirectory(src/app)


option(RUN_TESTS "Run tests" OFF)
if (RUN_TESTS)
    enable_testing()
    add_subdirectory(src/tests)
endif()