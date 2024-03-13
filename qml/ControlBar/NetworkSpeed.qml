import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    // 定义一个自定义组件
    Component {
        id: speedComponent
        Rectangle {
            Text {
                id: text1
                width: 102
                height: 13
                text: qsTr("Text")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: image.right
                font.pixelSize: 12
                anchors.leftMargin: 6
            }

            Image {
                id: image
                width: 12
                height: 12
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: "qrc:/qtquickplugin/images/template_image.png"
                anchors.leftMargin: 8
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    // 使用自定义组件
    Item {
        anchors.centerIn: parent
        Loader {
            sourceComponent: speedComponent
        }
    }
}
