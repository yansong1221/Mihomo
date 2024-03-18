import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import "./Proxies"
import Clash.Meta

FluContentPage {
    id: root1
    title: qsTr("Proxies")

    Component.onCompleted: {
        proxiesModel.reload();
    }
    ProxiesGroupModel {
        id: proxiesModel
    }
    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: mode
            implicitHeight: 80
            Layout.fillWidth: true
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

        ListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            orientation: ListView.Vertical
            ScrollBar.vertical: FluScrollBar {}
            delegate: GroupItem {
                width: listView.width
                model: display

                Component.onCompleted: {
                }
            }
            model: proxiesModel
        }
    }
}
