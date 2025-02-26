cmake_minimum_required(VERSION 3.15)

project(audio_tell)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(whisper.cpp)

include_directories(
    # ${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp-v1.5.0/examples
)


add_executable(${PROJECT_NAME})


target_sources(${PROJECT_NAME} PRIVATE
    main.cc
)


target_link_libraries(${PROJECT_NAME}
    # common
    # whisper # remember to copy dll or so to bin folder
)