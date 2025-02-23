// VideoPlayer.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

import "../common"

Item {
    id: playerRoot
    width: 1280
    height: 720

    // 暴露的接口
    property url source: ""
    property alias position: mediaPlayer.position
    property alias duration: mediaPlayer.duration
    property bool isPlaying: mediaPlayer.playbackState === MediaPlayer.PlayingState

    // 视频显示区域
    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        filters: [ videoFilter ] // 可添加视频滤镜
    }

    // 媒体播放器核心
    MediaPlayer {
        id: mediaPlayer
        source: playerRoot.source
        videoOutput: videoOutput
        loops: MediaPlayer.Infinite
        volume: volumeSlider.value

        onPositionChanged: {
            // 同步到时间轴（需要连接时间轴信号）
            timeline.currentTime = position
        }
    }

    // 视频处理滤镜（示例：灰度滤镜）
    ShaderEffect {
        id: videoFilter
        property variant source: ShaderEffectSource {
            sourceItem: videoOutput
            live: true
        }

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;
            uniform sampler2D source;
            void main() {
                vec4 color = texture2D(source, qt_TexCoord0);
                float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
                gl_FragColor = vec4(vec3(gray), color.a);
            }"
    }

    // 控制面板
    Rectangle {
        width: parent.width
        height: 60
        anchors.bottom: parent.bottom
        color: "#80000000"

        Row {
            anchors.centerIn: parent
            spacing: 20

            // 播放/暂停按钮
            IconButton {
//                iconSource: mediaPlayer.playbackState === MediaPlayer.PlayingState ? "qrc:icons/32x32/icon_set_cloud32_nor.png" : "icons/32x32/icon_set_lamp32.png"
                onClicked: togglePlayback()
            }

            // 进度条
            Slider {
                id: seekSlider
                width: 600
                from: 0
                to: mediaPlayer.duration
                value: mediaPlayer.position

                onMoved: {
                    mediaPlayer.pause()
                    mediaPlayer.seek(value)
                }
            }

            // 时间显示
            Label {
                text: formatTime(mediaPlayer.position) + " / " + formatTime(mediaPlayer.duration)
                color: "white"
                font.pixelSize: 14
            }

            // 音量控制
            Slider {
                id: volumeSlider
                width: 100
                from: 0
                to: 1
                value: 0.5
            }
        }
    }

    // 操作方法
    function togglePlayback() {
        if (mediaPlayer.playbackState === MediaPlayer.PlayingState) {
            mediaPlayer.pause()
        } else {
            mediaPlayer.play()
        }
    }

    function seekToTime(ms) {
        mediaPlayer.seek(ms)
    }

    function formatTime(ms) {
        let seconds = Math.floor(ms / 1000)
        let minutes = Math.floor(seconds / 60)
        seconds = seconds % 60
        return `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`
    }

    // 与时间轴集成
    Connections {
        target: timeline
        onCurrentTimeChanged: {
            if (!seekSlider.pressed) {
                mediaPlayer.seek(timeline.currentTime)
            }
        }
    }
}
