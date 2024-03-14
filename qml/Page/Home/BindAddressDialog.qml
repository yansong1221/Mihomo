import QtQuick
import QtQuick.Layouts
import FluentUI

FluContentDialog {
    id: root
    title: qsTr("更改绑定地址")
    negativeText: qsTr("取消")
    positiveText: qsTr("确定")
    contentDelegate: Component {
        Item {
            implicitWidth: parent.width
            implicitHeight: 80
            ColumnLayout {
                //anchors.centerIn: parent
                //Layout.topMargin:40
                anchors.fill: parent
                FluText {
                    color: "#198DAB"
                    Layout.fillWidth: true
                    Layout.leftMargin: 40
                    Layout.rightMargin: 40
                    text: qsTr("允许LAN只会绑定到您设置的地址，*表示所有接口")
                }
                FluText {
                    text: qsTr("新绑定地址")
                    Layout.fillWidth: true
                    Layout.leftMargin: 40
                    Layout.rightMargin: 40
                    Layout.topMargin: 5
                }
                FluTextBox {
                    Layout.fillWidth: true
                    Layout.leftMargin: 40
                    Layout.rightMargin: 40
                    Layout.topMargin: 5
                }
            }
        }
    }
    onNegativeClicked: {
        console.log(qsTr("Click the Cancel Button"));
    }
    onPositiveClicked: {
        console.log(qsTr("Click the OK Button"));
    }
}
