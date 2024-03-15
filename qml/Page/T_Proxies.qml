import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import "./Proxies"

FluContentPage {
    id: root1
    title: qsTr("Proxies")

    Rectangle {
        id: mode
        implicitHeight: 80
        width: parent.width
        color: "red"
        RowLayout {
            anchors.centerIn: parent
            spacing: 20
            ModeButton {
                text: qsTr("Global")
                checkable: true
                darkSource: "qrc:/assets/icons/dark/network-port.svg"
                lightSource: "qrc:/assets/icons/light/network-port.svg"
                onClicked:
                //checked = !checked;
                {
                }
            }
            ModeButton {
                text: qsTr("Rule")
                checkable: true
                darkSource: "qrc:/assets/icons/dark/network-port.svg"
                lightSource: "qrc:/assets/icons/light/network-port.svg"
                onClicked:
                //checked = !checked;
                {
                }
            }
            ModeButton {
                text: qsTr("Direct")
                checkable: true
                darkSource: "qrc:/assets/icons/dark/network-port.svg"
                lightSource: "qrc:/assets/icons/light/network-port.svg"
                onClicked:
                //checked = !checked;
                {
                }
            }
        }
    }
    Item {
        id: root
        width: 360
        height: 300

        // 用Rectangle作为背景
        Rectangle {
            id: bg
            anchors.fill: parent
            color: "#353535"
        }

        // ListView
        ListView {
            id: listView
            anchors.fill: parent
            delegate: listDelegate
            model: listModel
        }

        // ListModel
        ListModel {
            id: listModel
            ListElement {
                icon: "qrc:/CaretRight.png"
                name: qsTr("情况1")
            }
            ListElement {
                icon: "qrc:/CaretRight.png"
                name: qsTr("情况2")
            }
        }

        // delegate
        Component {
            id: listDelegate
            FluExpander {
                headerText: qsTr("Open a radio box")
                Layout.topMargin: 20
                Item {
                    anchors.fill: parent
                    FluRadioButtons {
                        spacing: 8
                        anchors {
                            top: parent.top
                            left: parent.left
                            topMargin: 15
                            leftMargin: 15
                        }
                        FluRadioButton {
                            text: "Radio Button_1"
                        }
                        FluRadioButton {
                            text: "Radio Button_2"
                        }
                        FluRadioButton {
                            text: "Radio Button_3"
                        }
                    }
                }
            }
        }
    }
}
