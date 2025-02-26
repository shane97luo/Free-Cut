import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

Rectangle {
    id: timeline
    width: 800
    height: 120
    color: "#1a1a1a"

    property int half_min: 30
    property int item_width_per_half_min: 50

    // 当前时间属性（单位：秒）
    property real currentTime: 0

    property int totalDuration: {
        let sum = 0;
        for (let clip of videoClips) {
            sum += clip.duration;
        }
        return sum;
    }

    // 可滚动视频轨道
    Flickable {
        id: flick

        anchors.fill: parent

        contentWidth: (item_width_per_half_min) * (totalDuration / half_min) // 动态内容宽度 ， 30 秒一帧， 40 长度

        // interactive: false // 禁用直接滚动

        ScrollBar.horizontal: ScrollBar {}

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            // 时间轴刻度尺
            Rectangle {
                id: ruler
                height: 30
                // anchors.top: parent.top

                color: "transparent"

                // 主刻度线
                Repeater {
                    model: timeline.totalDuration / half_min / 2 // min
                    delegate: Rectangle {
                        width: 1
                        height: 15
                        color: "#ffffff"
                        x: index * (item_width_per_half_min * 2)
                        y: 10

                        Text {
                            text: {
                                let hours = Math.floor(index / 4);
                                let minutes = (index % 4) * 15`${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}`;
                            }
                            color: "white"
                            font.pixelSize: 10
                            anchors.top: parent.bottom
                        }
                    }
                }

                // 次刻度线（每分钟）
                Repeater {
                    model: timeline.totalDuration // 每分钟一个次刻度
                    delegate: Rectangle {
                        width: 1
                        height: 8
                        color: "#666666"
                        x: index * (timeline.width)
                        y: 17
                    }
                }
            }

            // 视频片段容器
            Row {
                spacing: 0.1

                Repeater {
                    model: videoClips
                    delegate: Rectangle {

                        width: (modelData.duration / totalDuration) * flick.contentWidth
                        height: 60
                        color: "#4d4d4d"
                        radius: 3

                        // 文件名显示
                        Text {
                            text: modelData.name
                            color: "white"
                            font.pixelSize: 10
                            anchors.centerIn: parent
                        }

                        // 持续时间标签
                        Text {
                            text: formatDuration(modelData.duration)
                            color: "white"
                            font.pixelSize: 8
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                        }
                    }
                }
            }
        }
    }

    // 时间指示器系统
    Rectangle {
        id: timeIndicator
        width: 2
        height: parent.height
        color: "white"
        x: currentTime * (flick.contentWidth / totalDuration)

        Rectangle {
            id: cursor
            width: 6
            height: 10
            color: "transparent"

            Shape {
                anchors.fill: parent

                ShapePath {
                    fillColor: "gray"
                    strokeColor: "transparent"

                    PathMove {
                        x: 0
                        y: 0
                    }
                    PathLine {
                        x: cursor.width
                        y: 0
                    }
                    PathLine {
                        x: cursor.width
                        y: cursor.height - 3
                    }
                    PathLine {
                        x: cursor.width / 2
                        y: cursor.height
                    }
                    PathLine {
                        x: 0
                        y: cursor.height - 3
                    }
                }
            }
        }

        // 时间标签
        Text {
            text: formatTime(currentTime)
            color: "red"
            font.pixelSize: 12
            anchors.bottom: parent.top
        }

        // 拖拽交互
        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: Drag.XAxis
            drag.minimumX: 0
            drag.maximumX: timeline.width

            onPositionChanged: {
                // 计算新时间并更新视频位置
                currentTime = (timeIndicator.x * totalDuration) / flick.contentWidth;
                flick.contentX = Math.max(0, timeIndicator.x - timeline.width / 2);
            }
        }
    }

    // 时间格式化函数
    function formatTime(seconds) {
        let hrs = Math.floor(seconds / 3600);
        let min = Math.floor((seconds % 3600) / 60);
        let sec = Math.floor(seconds % 60);
        return `${hrs.toString().padStart(2, '0')}:${min.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
    }

    // 持续时间格式化
    function formatDuration(seconds) {
        let min = Math.floor(seconds / 60);
        let sec = seconds % 60;
        return `${min.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
    }

    // 数据结构
    property var videoClips: [
        {
            name: "freecut_Video01.mp4",
            duration: 300 // 5分钟
        },
        {
            name: "freecut_Video02.mp4",
            duration: 600 // 10分钟
        },
        {
            name: "freecut_Pe02Gif.mp4",
            duration: 150 // 2分30秒
        }
    ]

    Component.onCompleted: {
        //
        console.log("Total Duration:", totalDuration);
    }
}
