

set(gif_test gen_gif)

add_executable(${gif_test}
    gen_gif.cc
    ffmpeg_wrapper.h
    ffmpeg_wrapper.cc
)


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

find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_package(fmt REQUIRED)



target_link_libraries(${gif_test}
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${SWSCALE_LIBRARY}
    opencv_core
    opencv_calib3d
    opencv_imgproc
    opencv_dnn
    opencv_features2d
    opencv_flann
    opencv_imgcodecs
    gif
    png
    fmt
)

