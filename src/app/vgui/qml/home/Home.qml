import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15

Item {

    id: root


    RowLayout {

        anchors.fill: parent
        spacing: 0

        SideBar {
            id: side
            Layout.preferredWidth: 220  // 固定宽度
            Layout.fillHeight: true     // 填满垂直空间

            bgcolor: "#333333"

            onPageChange: {
                cpageChange(page)
            }
        }

        StackView {
            id: mainStackview

            Layout.fillWidth: true      // 占据剩余水平空间
            Layout.fillHeight: true     // 填满垂直空间

            Welcome {
                bgcolor: "#2A1B2D"
            }
        }

    }


    function cpageChange(name)
    {
        console.log("page", name)

        if(mainStackview.depth > 1)
            mainStackview.pop()

        if(name === "welcome")
        {
            mainStackview.push("qrc:/home/Welcome.qml")
        }

        if(name === "cloudSpace")
        {
            mainStackview.push("qrc:/home/CloudSpace.qml")
        }
        if(name === "rss")
        {
            mainStackview.push("qrc:/home/Rss.qml")
        }
        if(name === "activity")
        {
            mainStackview.push("qrc:/home/Activities.qml")
        }

    }
}
