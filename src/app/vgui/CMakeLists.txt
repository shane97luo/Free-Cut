
cmake_minimum_required(VERSION 3.21)

project(FreeCutGui VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets Multimedia REQUIRED )
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets Multimedia REQUIRED)

# 查找 FFmpeg 头文件目录
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)

# 查找 FFmpeg 库文件
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVCODEC_LIBRARY avcodec)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARY swscale)


set(VGUI_SOURCES
    main.cc
    video_decoder.cc
    video_decoder.h
    ffmpeg_wrapper.h
    ffmpeg_wrapper.cc
    qml/qml.qrc
    resources/resources.qrc
)

qt5_wrap_cpp(VGUI_SOURCES 
    video_decoder.h
)


add_executable(${PROJECT_NAME})


target_sources(${PROJECT_NAME} PRIVATE
    ${VGUI_SOURCES}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${SWSCALE_LIBRARY}
    fmt
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


# cmake_minimum_required(VERSION 3.5)

# project(untitled VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
# find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# set(PROJECT_SOURCES
#         main.cpp
#         mainwindow.cpp
#         mainwindow.h
# )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(untitled
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
# else()
#     if(ANDROID)
#         add_library(untitled SHARED
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(untitled
#             ${PROJECT_SOURCES}
#         )
#     endif()
# endif()

# target_link_libraries(untitled PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# set_target_properties(untitled PROPERTIES
#     MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(untitled)
# endif()


