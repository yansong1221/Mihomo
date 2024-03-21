import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import "./Proxies"
import Clash.Meta

FluPage {
    id: root1

    clip: true

    //title: qsTr("Proxies")

    Component.onCompleted: {
        proxiesModelFilter.reload();
    }
    ProxiesGroupModelFilter {
        id: proxiesModelFilter
    }
    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            implicitHeight: 80

            ListView {
                id: modeListView
                anchors.centerIn: parent
                width: itemWidth * model.count
                height: 30
                clip: true
                highlightMoveDuration: 200
                currentIndex: {
                    for (var i = 0; i < model.count; ++i) {
                        var item = model.get(i);
                        if (item.content === ClashRuntimeConfigure.mode)
                            return i;
                    }
                    return -1;
                }
                onCurrentIndexChanged: {
                    if (currentIndex == -1)
                        proxiesModelFilter.filerMode = "";
                    else
                        proxiesModelFilter.filerMode = model.get(currentIndex).content;
                }

                //interactive: false

                property int itemWidth: 80
                property int itemSapcing: 3
                property int itemRadius: 4

                orientation: ListView.Horizontal
                model: ListModel {
                    ListElement {
                        title: qsTr("Global")
                        content: "global"
                    }
                    ListElement {
                        title: qsTr("Rule")
                        content: "rule"
                    }
                    ListElement {
                        title: qsTr("Direct")
                        content: "direct"
                    }
                }
                delegate: Item {
                    width: modeListView.itemWidth
                    height: modeListView.height
                    Rectangle {

                        anchors.fill: parent
                        anchors.leftMargin: modeListView.itemSapcing

                        color: FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
                        border.color: FluTheme.dark ? Window.active ? Qt.rgba(55 / 255, 55 / 255, 55 / 255, 1) : Qt.rgba(45 / 255, 45 / 255, 45 / 255, 1) : Qt.rgba(226 / 255, 229 / 255, 234 / 255, 1)

                        radius: modeListView.itemRadius
                        FluText {
                            anchors.centerIn: parent
                            text: model.title
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                ClashRuntimeConfigure.mode = model.content;
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
                    z: modeListView.z + 2
                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: modeListView.itemSapcing

                        color: FluTheme.itemPressColor
                        radius: modeListView.itemRadius
                        Rectangle {
                            width: parent.width
                            anchors.bottom: parent.bottom
                            height: 3
                            color: "#0066b4"
                            radius: 2
                        }
                    }
                }
            }
        }
        SwipeView {
            id: view
            interactive: false
            currentIndex: modeListView.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true
            GroupItem {
                id: globalGroupItem
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            RuleModePage {
                dataModel: proxiesModelFilter
            }
            DirectModePage {
            }
        }
    }
}
