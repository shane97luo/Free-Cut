import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3

import "home"
import "workspace"
import "qrc:/javascripts/global.js" as GlobalJs

Window {
    visible: true

    width: 1024
    height: 768

    title: qsTr("FreeCutStudio")

    Loader {
        id: pageLoader

        anchors.fill: parent

        sourceComponent: editorPage
    }

    Connections {
        target: pageLoader.item

        function onQuit(name) {
            console.log("quit page:", name);
            if (name === "home") {
                console.log("enter editor page");
                pageLoader.sourceComponent = editorPage;
            } else if (name === "editor") {
                console.log("enter home page");
                pageLoader.sourceComponent = homePage;
            } else {
                console.error("unknown page name");
            }
        }
    }

    Component {
        id: homePage

        Home {
            anchors.fill: parent

            signal quit(string name)
        }
    }

    Component {
        id: editorPage

        WorkSpace {
            anchors.fill: parent

            signal quit(string name)
        }
    }
}
