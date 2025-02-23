#include "video_decoder.h"

#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    // print the version of Qt
    qDebug() << "Qt version: " << QT_VERSION_STR;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("FreeCut Studio");
    QCoreApplication::setApplicationName("FreeCutStudio");

    // 创建解码器和转换器实例
    VideoDecoder videoDecoder;
    // GifConverter gifConverter;
    // gifConverter.setDecoder(&videoDecoder); // 关联解码器

    QQmlApplicationEngine engine;

    // 注册到 QML context
    engine.rootContext()->setContextProperty("videoDecoder", &videoDecoder);
    // engine.rootContext()->setContextProperty("gifConverter", &gifConverter);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreated, &app,
            [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
