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
        Item {
            Layout.fillWidth: true
            implicitHeight: 80

            ListView {
                id: tabListView
                anchors.centerIn: parent
                //width: 50 * model.count
                width: parent.width
                height: parent.height
                clip: true
                property int previousIndex: -1

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
                    width: 50
                    height: 30
                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "transparent"

                        //border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: model.title
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tabListView.previousIndex = tabListView.currentIndex;
                                tabListView.currentIndex = index;
                            }
                        }
                    }
                }
                highlight: Rectangle {
                    //高亮
                    //width: 100
                    //height: 30
                    color: "black"
                    x: tabListView.currentItem.x
                    Behavior on x {
                        //点击选中的时候会有个动画
                        SpringAnimation {
                            spring: 3
                            damping: 0.2
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
