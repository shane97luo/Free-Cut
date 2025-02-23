
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

