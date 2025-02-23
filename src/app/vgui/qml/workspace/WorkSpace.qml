import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3 as OriDialogs
import Qt.labs.platform 1.1 as LabControls

import "qrc:/javascripts/global.js" as GlobalJs

Item {

    width: 800
    height: 600

    MediaPlayer {
        id: mediaPlayer

        autoPlay: true

        onError: {
            console.log("Error: ", errorString);
        }

        onPlaybackStateChanged: {
            switch (playbackState) {
            case MediaPlayer.PlayingState:
                playPauseBtn.text = "暂停";
                break;
            case MediaPlayer.PausedState:
                playPauseBtn.text = "播放";
                break;
            case MediaPlayer.StoppedState:
                playPauseBtn.text = "开始";
                break;
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // 视频预览区域
        VideoOutput {
            id: videoOutput

            Layout.fillWidth: true
            Layout.fillHeight: true

            source: mediaPlayer
        }

        Slider {
            id: progressBar
            Layout.fillWidth: true
            Layout.preferredHeight: 15

            from: 0
            to: mediaPlayer.duration > 0 ? mediaPlayer.duration : 1
            value: mediaPlayer.position

            enabled: mediaPlayer.seekable

            property bool seeking: false

            onPressedChanged: {
                console.log("pressed: ", value);
                mediaPlayer.seek(value);
            }

            // 拖动进度条时更新播放位置
            onMoved: {
                console.log("moved: ", value);
                mediaPlayer.seek(value);
            }

            // 显示时间信息
            ToolTip {
                parent: progressBar.handle
                visible: progressBar.pressed
                text: formatTime(progressBar.value) + " / " + formatTime(mediaPlayer.duration)
            }
        }

        // 双滑块范围选择组件
        RangeSlider {
            id: rangeSlider

            Layout.fillWidth: true
            Layout.preferredHeight: 20

            from: 0
            to: mediaPlayer.duration
            stepSize: 100
            first.value: 0.0
            second.value: 0.4

            // 实时预览处理
            first.onMoved: {
                console.log("first.onMoved: ", first.value);
            }

            second.onMoved: {
                console.log("second.onMoved: ", second.value);
            }
        }

        Row {

            Layout.alignment: Qt.AlignHCenter

            spacing: 20

            Label {
                text: "开始: " + formatTime(rangeSlider.first.value)
                color: "#2196F3"
            }

            Label {
                text: "结束: " + formatTime(rangeSlider.second.value)
                color: "#2196F3"
            }

            Label {
                text: "时长: " + formatTime(rangeSlider.second.value - rangeSlider.first.value)
                color: "#666"
            }
        }

        Row {

            Layout.alignment: Qt.AlignHCenter

            spacing: 15

            Button {
                id: playPauseBtn
                text: "播放"
                onClicked: togglePlayState()
            }

            Button {
                text: "选择文件"
                onClicked: fileDialog.open()
            }

            Button {

                LabControls.FolderDialog {
                    id: saveAsDialog

                    title: qsTr("Save As")

                    onAccepted: {
                        console.log("saveAsDialog: ", saveAsDialog.currentFolder, "转换gif", rangeSlider.first.value, rangeSlider.second.value);

                        var begin = rangeSlider.first.value / 1000.0;
                        var end = rangeSlider.second.value / 1000.0;
                        videoDecoder.convert2Gif(begin, end, saveAsDialog.currentFolder);
                    }
                }

                text: qsTr("转换gif")
                onClicked: {
                    if (mediaPlayer.status === MediaPlayer.NoMedia) {
                        console.log("请先选择视频文件");
                    } else {
                        saveAsDialog.open();
                    }
                }
            }

            Label {
                text: formatTime(mediaPlayer.position) + " / " + formatTime(mediaPlayer.duration)
                anchors {
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        ProgressBar {
            id: conversionProgress
            Layout.fillWidth: true
            visible: false
            from: 0
            to: 100
        }
    }

    // 文件选择对话框
    OriDialogs.FileDialog {
        id: fileDialog
        onAccepted: videoDecoder.loadVideo(fileDialog.fileUrl)
    }

    // 后端对象连接
    Connections {
        target: videoDecoder

        function onLoaded(durationMs) {
            console.log("Loaded video: ", fileDialog.fileUrl, "duration: ", durationMs);
            mediaPlayer.source = fileDialog.fileUrl;
        }
    }

    // 时间格式化函数
    function formatTime(milliseconds) {
        if (!milliseconds) {
            return "00:00";
        }

        let seconds = Math.floor(milliseconds / 1000);
        let minutes = Math.floor(seconds / 60);
        seconds = seconds % 60;
        return `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`;
    }

    // 播放/暂停切换
    function togglePlayState() {
        console.log("togglePlayState", mediaPlayer.playbackState);
        if (mediaPlayer.playbackState === MediaPlayer.PlayingState) {
            mediaPlayer.pause();
            playPauseBtn.text = "播放";
        } else {
            mediaPlayer.play();
            playPauseBtn.text = "暂停";
        }
    }

    Component.onCompleted: {
        videoOutput.autoOrientation = true;
        videoOutput.fillMode = VideoOutput.PreserveAspectFit;

        console.log("dsd", GlobalJs.obejctEmpty(mediaPlayer.source), mediaPlayer.status === MediaPlayer.NoMedia);

        for (let key in mediaPlayer.source) {
            console.log(key, mediaPlayer.source[key]);
        }
    }
}
