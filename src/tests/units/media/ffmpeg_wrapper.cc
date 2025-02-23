#include "ffmpeg_wrapper.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <iterator>
#include <cstdio>
#include <memory>
// #include <format>
#include <fmt/core.h> // 替代 <format>

std::string cmd =
        R"(ffmpeg -ss 00:00:1 -t 4 -i  media.mp4 -vf "fps=15,scale=640:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" -loop 0 outputdedd.gif)";

// {0} begin timestamp, {1} continous time, {2} input file, {3} output file
std::string cmd_format =
        R"(ffmpeg -ss 00:00:{0} -t {1} -i  {2} -vf "fps=15,scale=640:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" -loop 0 {3})";

static std::string runProgram(const std::string &cmd, const std::string &params)
{

    auto command = cmd + " " + params;

    FILE *pipe = popen(command.c_str(), "r");

    std::string result;
    try {
        char c;
        // 逐个字符读取命令输出
        while ((c = std::fgetc(pipe)) != EOF) {
            result += c;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);

    return result;
}

FFmpegWrapper::FFmpegWrapper() { }

FFmpegWrapper::~FFmpegWrapper() { }

std::string FFmpegWrapper::convertVideo2Gif(const std::string &input_file,
                                            const std::string &output_file, double start_time,
                                            double end_time)
{
    auto cmd = fmt::format(
            R"(ffmpeg -ss 00:00:{0} -t {1} -i  {2} -vf "fps=15,scale=640:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" -loop 0 {3})",
            start_time, end_time - start_time, input_file.data(), output_file.data());

    runProgram(cmd, "");

    return "";
}
