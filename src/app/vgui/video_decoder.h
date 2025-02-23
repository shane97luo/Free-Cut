#ifndef VIDEO_DECODER_H
#define VIDEO_DECODER_H

#include <QObject>
#include <QUrl>

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class VideoDecoder : public QObject
{
    Q_OBJECT
public:
    explicit VideoDecoder(QObject *parent = nullptr);

    ~VideoDecoder();

    Q_INVOKABLE bool loadVideo(const QUrl &fileUrl); // 支持QML调用

    Q_INVOKABLE void convert2Gif(double begintime, double endtime, const QUrl &targetDir);

    QUrl source() const { return _source; }

signals:
    void sourceChanged();

    void loaded(qint64 durationMs);

    void error(const QString &message);

private:
    bool initCodec();

    void cleanup();

    AVFormatContext *_formatCtx{ nullptr };

    AVCodecContext *_codecCtx{ nullptr };

    SwsContext *_swsCtx{ nullptr };

    int _videoStream = { -1 };

    int _duration{ 0 };

    QUrl _source;
};

#endif // VIDEO_DECODER_H
