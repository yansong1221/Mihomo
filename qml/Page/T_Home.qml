import QtQuick
import QtQuick.Layouts
import FluentUI
import "./Home"

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
    Component {
        id: bindAddressDialog
        BindAddressDialog {
        }
    }

    ColumnLayout {
        spacing: 5
        Layout.topMargin: 20
        Layout.fillWidth: true

        HomeItemArea {
            leftItems: FluObject {
                Image {
                    source: FluTheme.dark ? "qrc:/assets/icons/dark/network-port.svg" : "qrc:/assets/icons/light/network-port.svg"
                    Layout.maximumWidth: 16
                    Layout.maximumHeight: 16
                    mipmap: true
                    fillMode: Image.PreserveAspectFit
                }
                FluText {
                    Layout.fillWidth: true
                    text: qsTr("Port")
                    font: FluTextStyle.BodyStrong
                }
            }
            rightItems: FluObject {
                FluLoader {
                    sourceComponent: switchButton
                    onLoaded: {
                        item.onCheckedChanged.connect(function () {
                            console.log("Signal emitted from loaded component");
                        });
                    }
                }
            }
        }
        HomeItemArea {
            Layout.fillWidth: true
            height: 50

            leftItems: FluObject {

                FluText {
                    text: qsTr("Allow LAN")
                    font: FluTextStyle.BodyStrong
                }
            }

            rightItems: FluObject {
                FluTextButton {
                    text: "绑定地址:*"
                    onClicked: {
                        var dialog = bindAddressDialog.createObject(parent);
                        dialog.open();
                    }
                }

                FluLoader {
                    id: allowLanSwitch
                    sourceComponent: switchButton
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
