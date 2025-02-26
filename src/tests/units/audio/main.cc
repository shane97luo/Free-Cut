#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// 定义一个函数来执行 Whisper 命令
std::string runWhisper(const std::string &audioPath, const std::string &modelPath)
{
    std::string command = "./main -m " + modelPath + " -f " + audioPath + " -l zh";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute command." << std::endl;
        return "";
    }
    std::vector<char> buffer(128);
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}

int main()
{
    // 音频文件路径
    std::string audioPath = "path/to/your/audio.wav";
    // 模型文件路径
    std::string modelPath = "models/ggml-base.bin";

    // 调用 Whisper 进行语音识别
    std::string transcription = runWhisper(audioPath, modelPath);

    // 输出识别结果
    std::cout << "Transcription: " << transcription << std::endl;

    return 0;
}