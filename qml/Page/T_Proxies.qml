import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import "./Proxies"
import Clash.Meta

Item {
    id: root1
    //title: qsTr("Proxies")

    Component.onCompleted: {
        proxiesModel.reload();
    }
    ProxiesGroupModel {
        id: proxiesModel
    }
    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            implicitHeight: 80

            ListView {
                id: tabListView
                anchors.centerIn: parent
                width: itemWidth * model.count
                height: 30
                clip: true
                highlightMoveDuration: 200
                //interactive: false

                property int itemWidth: 80
                property int itemSapcing: 3
                property int itemRadius: 4

                orientation: ListView.Horizontal
                model: ListModel {
                    ListElement {
                        title: qsTr("Global")
                        content: "Content of Tab 1"
                    }
                    ListElement {
                        title: qsTr("Rule")
                        content: "Content of Tab 2"
                    }
                    ListElement {
                        title: qsTr("Direct")
                        content: "Content of Tab 3"
                    }
                }
                delegate: Item {
                    width: tabListView.itemWidth
                    height: tabListView.height
                    Rectangle {

                        anchors.fill: parent
                        anchors.leftMargin: tabListView.itemSapcing

                        color: FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
                        border.color: FluTheme.dark ? Window.active ? Qt.rgba(55 / 255, 55 / 255, 55 / 255, 1) : Qt.rgba(45 / 255, 45 / 255, 45 / 255, 1) : Qt.rgba(226 / 255, 229 / 255, 234 / 255, 1)

                        radius: tabListView.itemRadius
                        FluText {
                            anchors.centerIn: parent
                            text: model.title
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled :true
                            onClicked: {
                                tabListView.currentIndex = index;
                            }
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;
                            }

                            onExited: {
                                cursorShape = Qt.ArrowCursor;
                            }
                        }
                    }
                }
                highlight: Item {
                    z: tabListView.z + 2
                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: tabListView.itemSapcing

                        color: FluTheme.itemPressColor
                        radius: tabListView.itemRadius
                        Rectangle {
                            width: parent.width
                            anchors.bottom: parent.bottom
                            height: 3
                            color: "#0066b4"
                            radius:2
                        }
                    }

                }
            }
        }
        ListView {
            id: grouplistView
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            orientation: ListView.Vertical
            ScrollBar.vertical: FluScrollBar {
            }
            delegate: GroupItem {
                width: grouplistView.width
                model: display

                Component.onCompleted: {
                }
            }
            model: proxiesModel
        }
    }
}
