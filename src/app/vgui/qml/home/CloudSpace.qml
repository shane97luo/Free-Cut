import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {

    property alias bgcolor: bg.color

    Rectangle{
        id: bg
        color: "#616161"

        anchors.fill: parent
    }


    ColumnLayout {

        anchors.fill: parent
        spacing: 0

        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

        }

    }


}
