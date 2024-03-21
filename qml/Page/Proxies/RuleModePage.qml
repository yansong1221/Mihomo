import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI

Item {
    id: root

    property alias dataModel: nav_list.model

    Connections {
        id: modelEvent
        target: nav_list.model
        onModelReset: nav_list.resetCurrentIndex()
        onRowsInserted: nav_list.resetCurrentIndex()
    }

    RowLayout {
        anchors.fill: parent
        LayoutMirroring.enabled: true
        ListView {
            id: nav_list

            boundsBehavior: ListView.StopAtBounds
            Layout.fillHeight: true
            implicitWidth: 200
            clip: true
            //currentIndex: model && model.rowCount() > 0 ? 0 : -1
 
            ScrollBar.vertical: FluScrollBar {
            }

            delegate: Item {
                width: parent.width
                height: 40
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        nav_list.currentIndex = index;
                    }
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor;
                    }
                    onExited: {
                        cursorShape = Qt.ArrowCursor;
                    }
                }
                FluText {
                    anchors.centerIn: parent
                    text: display.groupName
                }
            }
            highlight: Item {
                z: nav_list.z + 2
                Rectangle {
                    anchors.fill: parent

                    //anchors.topMargin: tabListView.itemSapcing

                    color: FluTheme.itemPressColor
                    radius: 1.5
                    Rectangle {
                        width: 3
                        anchors.left: parent.left
                        height: parent.height
                        color: "#0066b4"
                        radius: 2
                    }
                }
            }

            onCurrentIndexChanged: {
                if (currentIndex == -1)
                    groupItem.dataModel = null;
                else {
                    var index = model.index(currentIndex, 0);
                    var item = model.data(index);
                    groupItem.dataModel = item;
                }
            }
            function resetCurrentIndex() {
                if (nav_list.currentIndex === -1) {
                    nav_list.currentIndex = 0; // 当模型中有新数据时，自动选择第一个条目
                }
            }
        }
        GroupItem {
            id: groupItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
