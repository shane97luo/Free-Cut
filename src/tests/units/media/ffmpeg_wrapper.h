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

    // "ffmpeg - ss 00 : 00 : 1 - t 4 - i media.mp4
    //         - vf
    //         "fps=15,scale=640:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse"
    //         - loop 0 outputdedd.gif "
};

#endif // _FFMPEG_WRAPPER_H_
