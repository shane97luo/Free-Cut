#ifndef _FFMPEG_WRAPPER_H_
#define _FFMPEG_WRAPPER_H_

#include <string>

class FFmpegWrapper
{
public:
    FFmpegWrapper();
    ~FFmpegWrapper();

    static std::string convertVideo2Gif(const std::string &input_file,
                                        const std::string &output_file, double start_time,
                                        double end_time);
};

#endif // _FFMPEG_WRAPPER_H_
