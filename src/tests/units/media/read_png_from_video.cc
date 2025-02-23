
#include <gif_lib.h>
#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/timestamp.h>
#include <libavformat/avio.h>
#include <libavdevice/avdevice.h>
#include <libpostproc/postprocess.h>
#include <libswresample/swresample.h>
}

// 保存帧为图像文件
void saveFrame(AVFrame *pFrame, int width, int height, int frameNumber)
{
    AVFormatContext *pFormatCtx = nullptr;
    avformat_alloc_output_context2(&pFormatCtx, nullptr, "image2", nullptr);
    if (!pFormatCtx) {
        std::cerr << "Could not create output context" << std::endl;
        return;
    }

    AVCodec *pCodec = avcodec_find_encoder(AV_CODEC_ID_PNG);
    if (!pCodec) {
        std::cerr << "Codec not found" << std::endl;
        return;
    }

    AVStream *pStream = avformat_new_stream(pFormatCtx, pCodec);
    if (!pStream) {
        std::cerr << "Could not create new stream" << std::endl;
        return;
    }

    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        std::cerr << "Could not allocate codec context" << std::endl;
        return;
    }

    pCodecCtx->codec_id = AV_CODEC_ID_PNG;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_RGB24;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->time_base = { 1, 25 };

    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        return;
    }

    AVFrame *pRGBFrame = av_frame_alloc();
    if (!pRGBFrame) {
        std::cerr << "Could not allocate RGB frame" << std::endl;
        return;
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pRGBFrame->data, pRGBFrame->linesize, buffer, AV_PIX_FMT_RGB24, width,
                         height, 1);

    SwsContext *sws_ctx =
            sws_getContext(width, height, (AVPixelFormat)pFrame->format, width, height,
                           AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
    sws_scale(sws_ctx, (const uint8_t *const *)pFrame->data, pFrame->linesize, 0, height,
              pRGBFrame->data, pRGBFrame->linesize);

    std::string filename = "frame_" + std::to_string(frameNumber) + ".png";
    if (avio_open(&pFormatCtx->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0) {
        std::cerr << "Could not open output file" << std::endl;
        return;
    }

    avformat_write_header(pFormatCtx, nullptr);

    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    int ret = avcodec_send_frame(pCodecCtx, pRGBFrame);
    if (ret < 0) {
        std::cerr << "Error sending frame to encoder" << std::endl;
        return;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(pCodecCtx, &pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            std::cerr << "Error receiving packet from encoder" << std::endl;
            break;
        }

        av_write_frame(pFormatCtx, &pkt);
        av_packet_unref(&pkt);
    }

    av_write_trailer(pFormatCtx);

    sws_freeContext(sws_ctx);
    av_frame_free(&pRGBFrame);
    av_free(buffer);
    avcodec_free_context(&pCodecCtx);
    if (!(pFormatCtx->oformat->flags & AVFMT_NOFILE))
        avio_closep(&pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
}

// 按指定时间点截图
void captureFramesAtTimestamps(const std::string &inputFilePath,
                               const std::vector<int64_t> &timestamps)
{
    av_register_all();
    avformat_network_init();

    AVFormatContext *pFormatCtx = nullptr;
    if (avformat_open_input(&pFormatCtx, inputFilePath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open input file" << std::endl;
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        std::cerr << "Could not find stream information" << std::endl;
        avformat_close_input(&pFormatCtx);
        return;
    }

    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cerr << "Could not find video stream" << std::endl;
        avformat_close_input(&pFormatCtx);
        return;
    }

    AVCodecParameters *pCodecParameters = pFormatCtx->streams[videoStreamIndex]->codecpar;
    AVCodec *pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if (!pCodec) {
        std::cerr << "Codec not found" << std::endl;
        avformat_close_input(&pFormatCtx);
        return;
    }

    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParameters) < 0) {
        std::cerr << "Could not copy codec parameters to codec context" << std::endl;
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return;
    }

    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return;
    }

    AVFrame *pFrame = av_frame_alloc();
    AVPacket *pPacket = av_packet_alloc();

    int frameCount = 0;
    int timestampIndex = 0;
    while (av_read_frame(pFormatCtx, pPacket) >= 0) {
        if (pPacket->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(pCodecCtx, pPacket) < 0) {
                std::cerr << "Error sending packet to decoder" << std::endl;
                continue;
            }

            while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
                int64_t pts = pFrame->pts;
                if (timestampIndex < timestamps.size() && pts >= timestamps[timestampIndex]) {
                    saveFrame(pFrame, pCodecCtx->width, pCodecCtx->height, frameCount);
                    frameCount++;
                    timestampIndex++;
                }
            }
        }
        av_packet_unref(pPacket);
    }

    av_frame_free(&pFrame);
    av_packet_free(&pPacket);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
}

// 按指定时间间隔截图
void captureFramesAtInterval(const std::string &inputFilePath, int intervalSeconds)
{
    av_register_all();
    avformat_network_init();

    AVFormatContext *pFormatCtx = nullptr;
    if (avformat_open_input(&pFormatCtx, inputFilePath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open input file" << std::endl;
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        std::cerr << "Could not find stream information" << std::endl;
        avformat_close_input(&pFormatCtx);
        return;
    }

    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cerr << "Could not find video stream" << std::endl;
        avformat_close_input(&pFormatCtx);
        return;
    }

    AVCodecParameters *pCodecParameters = pFormatCtx->streams[videoStreamIndex]->codecpar;
    AVCodec *pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if (!pCodec) {
        std::cerr << "Codec not found" << std::endl;
        avformat_close_input(&pFormatCtx);
        return;
    }

    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParameters) < 0) {
        std::cerr << "Could not copy codec parameters to codec context" << std::endl;
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return;
    }

    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return;
    }

    AVFrame *pFrame = av_frame_alloc();
    AVPacket *pPacket = av_packet_alloc();

    int frameCount = 0;
    int64_t nextCaptureTime = 0;
    AVRational timeBase = pFormatCtx->streams[videoStreamIndex]->time_base;
    while (av_read_frame(pFormatCtx, pPacket) >= 0) {
        if (pPacket->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(pCodecCtx, pPacket) < 0) {
                std::cerr << "Error sending packet to decoder" << std::endl;
                continue;
            }

            while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
                int64_t pts = pFrame->pts;
                int64_t currentTime = av_rescale_q(pts, timeBase, { 1, 1 });
                if (currentTime >= nextCaptureTime) {
                    saveFrame(pFrame, pCodecCtx->width, pCodecCtx->height, frameCount);
                    frameCount++;
                    nextCaptureTime = currentTime + intervalSeconds * AV_TIME_BASE;
                }
            }
        }
        av_packet_unref(pPacket);
    }

    av_frame_free(&pFrame);
    av_packet_free(&pPacket);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
}

int main(int argc, char *argv[])
{

    std::string inputFilePath = "media.mp4";
    // 按指定时间点截图示例
    std::vector<int64_t> timestamps = { 1 * AV_TIME_BASE, 5 * AV_TIME_BASE, 10 * AV_TIME_BASE };
    captureFramesAtTimestamps(inputFilePath, timestamps);

    // 按指定时间间隔截图示例
    int intervalSeconds = 2;
    captureFramesAtInterval(inputFilePath, intervalSeconds);

    // std::string output_filename = "output.gif";
    // std::string mp4_filename = "media.mp4";

    // int gif_error;
    // GifFileType *gif_file = EGifOpenFileName(output_filename.c_str(), false, &gif_error);

    // if (gif_file == NULL) {
    //     fprintf(stderr, "Could not open GIF file\n");
    //     return 1;
    // }

    // AVFormatContext *format_context = nullptr;

    // if (avformat_open_input(&format_context, mp4_filename.c_str(), nullptr, nullptr) != 0) {
    //     fprintf(stderr, "Could not open video file\n");
    // }

    // if (avformat_find_stream_info(format_context, nullptr) < 0) {
    //     fprintf(stderr, "Could not find stream information\n");
    // }

    // int video_stream_index =
    //         av_find_best_stream(format_context, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

    // EGifPutScreenDesc(gif_file, video_codec_context->width, video_codec_context->height, 256, 0,
    //                   NULL);

    std::cout << "Hello World!" << std::endl;

    return 0;
}