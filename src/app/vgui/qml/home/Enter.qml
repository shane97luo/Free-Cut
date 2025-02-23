import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Item {

    property alias bgcolor: bg.color

    signal enterWorkspace()

    Rectangle{
        id: bg

        anchors.fill: parent

        gradient: Gradient {
            orientation: Gradient.Horizontal // 设置水平方向
            GradientStop { position: 0.0; color: "red" }
            GradientStop { position: 1.0; color: "blue" }
        }


    }

    Text {
        id: name
        text: qsTr("Start")
        anchors.centerIn: parent
    }


    MouseArea{
        id: button

        anchors.fill: parent

        onClicked: {
            enterWorkspace()
        }

    }
}
