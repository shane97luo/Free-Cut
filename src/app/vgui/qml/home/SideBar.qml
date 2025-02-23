import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {

    property alias bgcolor: bg.color

    signal pageChange(var page)

    Rectangle{
        id: bg
        color: "#616161"

        anchors.fill: parent

    }


    ColumnLayout {

        anchors.fill: parent
        spacing: 0

        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            text: qsTr("Account")

            onClicked: {
                pageChange("welcome")
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
        }

        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            text: qsTr("Cloud Storage")

            onClicked: {
                pageChange("cloudSpace")
            }
        }

        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            text: qsTr("RSS")

            onClicked: {
                pageChange("rss")
            }
        }

        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            text: qsTr("activity")

            onClicked: {
                pageChange("activity")
            }

        }


        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }

}
