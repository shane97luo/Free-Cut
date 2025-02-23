
#include "video_decoder.h"

#include "ffmpeg_wrapper.h"
#include <QDebug>

VideoDecoder::VideoDecoder(QObject *parent) : QObject(parent)
{
    avformat_network_init();
}

VideoDecoder::~VideoDecoder()
{
    //
    cleanup();
}

bool VideoDecoder::loadVideo(const QUrl &fileUrl)
{

    qDebug() << "file: " << fileUrl;

    _source = fileUrl;

    const QString path = fileUrl.toLocalFile();

    //
    if (avformat_open_input(&_formatCtx, path.toUtf8().constData(), nullptr, nullptr) != 0) {
        emit error("无法打开文件");
        return false;
    }

    // Find stream info
    if (avformat_find_stream_info(_formatCtx, nullptr) < 0) {
        emit error("无法获取流信息");
        return false;
    }

    // Find video stream
    _videoStream = av_find_best_stream(_formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (_videoStream < 0) {
        emit error("未找到视频流");
        return false;
    }

    // Initialize codec
    if (!initCodec())
        return false;

    // Get duration
    AVStream *stream = _formatCtx->streams[_videoStream];
    _duration = (stream->duration * av_q2d(stream->time_base)) * 1000;

    emit loaded(_duration);
    emit sourceChanged();
    return true;
}

bool VideoDecoder::initCodec()
{
    AVStream *stream = _formatCtx->streams[_videoStream];
    const AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec) {
        emit error("不支持的编解码器");
        return false;
    }

    _codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(_codecCtx, stream->codecpar);

    if (avcodec_open2(_codecCtx, codec, nullptr) < 0) {
        emit error("无法初始化解码器");
        return false;
    }

    // Init SWScale for color conversion
    _swsCtx = sws_getContext(_codecCtx->width, _codecCtx->height, _codecCtx->pix_fmt,
                             _codecCtx->width, _codecCtx->height, AV_PIX_FMT_RGB32, SWS_BILINEAR,
                             nullptr, nullptr, nullptr);

    return true;
}

void VideoDecoder::convert2Gif(double begintime, double endtime, const QUrl &targetDir)
{

    qDebug() << Q_FUNC_INFO << "begintime: " << begintime << "endtime:" << endtime << " video_file"
             << _source.toLocalFile();
    std::string video_path = _source.toLocalFile().toStdString();

    auto target_file = (targetDir.toLocalFile() + "/grap.gif").toStdString();

    FFmpegWrapper::convertVideo2Gif(video_path, target_file, begintime, endtime);
}

void VideoDecoder::cleanup()
{
    if (_swsCtx) {
        sws_freeContext(_swsCtx);
        _swsCtx = nullptr;
    }
    if (_codecCtx) {
        avcodec_free_context(&_codecCtx);
        _codecCtx = nullptr;
    }
    if (_formatCtx) {
        avformat_close_input(&_formatCtx);
        _formatCtx = nullptr;
    }

    _videoStream = -1;
}
