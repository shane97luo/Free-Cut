// TrackDelegate.qml
import QtQuick 2.15

Rectangle {
    id: track
    property var clipModel
    property real timeScale: 1.0
    property color trackColor

    color: trackColor

    // 剪辑项
    Repeater {
        model: clipModel
        delegate: Rectangle {
            id: clip
            x: start * timeScale
            width: duration * timeScale
            height: parent.height - 4
            y: 2
            color: model.color
            radius: 3

            // 拖动处理
            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XAxis
                drag.minimumX: 0
                drag.maximumX: track.width - width

                onPositionChanged: {
                    // 更新模型中的开始时间
                    clipModel.setProperty(index, "start", Math.round(clip.x / timeScale))
                }
            }

            // 右侧调整手柄
            Rectangle {
                width: 8
                height: parent.height
                anchors.right: parent.right
                color: "white"
                opacity: 0.5

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.SizeHorCursor
                    drag.target: parent.parent
                    drag.axis: Drag.XAxis
                    drag.minimumX: 0
                    drag.maximumX: track.width

                    onPositionChanged: {
                        var newWidth = Math.max(10, clip.width + mouseX)
                        clip.width = newWidth
                        clipModel.setProperty(index, "duration", Math.round(newWidth / timeScale))
                    }
                }
            }
        }
    }
}