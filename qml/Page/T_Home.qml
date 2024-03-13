import QtQuick
import QtQuick.Layouts
import FluentUI

FluScrollablePage {
    id: root
    title: qsTr("Home")

    Component {
        id: switchButton
        FluToggleSwitch {
            text: checked ? qsTr("ON") : qsTr("OFF")
            width: 60
        }
    }
    ColumnLayout {
        spacing: 5
        Layout.topMargin: 20
        Layout.fillWidth: true

        FluArea {
            Layout.fillWidth: true
            height: 50
            RowLayout{
                Layout.fillWidth: true
                anchors{
                    fill:parent
                    leftMargin:40
                    rightMargin: 40
                }        
                FluText {
                    text: qsTr("Port")
                    font: FluTextStyle.BodyStrong
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                }

                FluLoader {
                    Layout.alignment: Qt.AlignRight
                    sourceComponent: switchButton
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    onLoaded: {
                        item.onCheckedChanged.connect(function () {
                            console.log("Signal emitted from loaded component");
                        });
                    }
                }
            }
        }
        FluArea {
            Layout.fillWidth: true
            height: 50

            FluContentDialog {
                id: dialog
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

            Item {
                anchors {
                    fill: parent
                    leftMargin: 40
                    rightMargin: 40
                }

                FluText {
                    text: qsTr("Allow LAN")
                    font: FluTextStyle.BodyStrong
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                }

                FluTextButton {
                    text: "绑定地址:*"
                    anchors {
                        right: allowLanSwitch.left
                        verticalCenter: parent.verticalCenter
                    }

                    onClicked: {
                        dialog.open();
                    }
                }

                FluLoader {
                    id: allowLanSwitch
                    Layout.alignment: Qt.AlignRight
                    sourceComponent: switchButton
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    onLoaded: {
                        item.onCheckedChanged.connect(function () {
                            console.log("Signal emitted from loaded component");
                        });
                    }
                }
            }
        }
    }
}
