
// Timeline.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: timelineRoot
    width: 800
    height: 400

    // 时间轴属性
    property real timeScale: 1.0      // 时间缩放因子
    property real maxDuration: 60000  // 最大时间（毫秒）
    property real currentTime: 0      // 当前播放头位置

    // 轨道数据模型
    property ListModel tracks: ListModel {
        ListElement {
            trackName: "Video 1"
            clips: [
                ListElement { start: 0; duration: 5000; color: "blue" },
                ListElement { start: 6000; duration: 3000; color: "green" }
            ]
        }
        ListElement {
            trackName: "Audio 1"
            clips: [
                ListElement { start: 0; duration: 8000; color: "orange" }
            ]
        }
    }

    // 时间标尺
    Rectangle {
        id: ruler
        width: parent.width
        height: 30
        color: "#333"

        Row {
            spacing: 50 * timeScale
            Repeater {
                model: maxDuration / 1000
                Label {
                    text: index + "s"
                    color: "white"
                    x: index * 1000 * timeScale
                }
            }
        }
    }

    // 轨道区域
    Flickable {
        id: flickable
        width: parent.width
        height: parent.height - ruler.height
        y: ruler.height
        contentWidth: maxDuration * timeScale
        contentHeight: tracks.count * 50
        clip: true

        // 轨道列表
        Column {
            spacing: 5
            Repeater {
                model: tracks
                delegate: TrackDelegate {
                    width: flickable.contentWidth
                    height: 45
                    trackColor: "#444"
                    clipModel: clips
                    timeScale: timelineRoot.timeScale
                }
            }
        }

        // 播放头
        Rectangle {
            id: playhead
            width: 2
            height: flickable.contentHeight
            color: "red"
            x: currentTime * timeScale
            y: 0
        }
    }

    // 缩放控制
    MouseArea {
        anchors.right: parent.right
        anchors.top: parent.top
        width: 30
        height: 30
        onWheel: {
            timeScale = Math.min(10, Math.max(0.1, timeScale * (wheel.angleDelta.y > 0 ? 1.1 : 0.9)))
        }
    }
}

