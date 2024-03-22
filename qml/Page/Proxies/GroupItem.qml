import FluentUI
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

FluArea {
    id: root

    property alias dataModel: view.model

    //contentHeight: view.contentHeight
    //headerText: dataModel.groupName
    ColumnLayout {

        anchors.fill: parent

        GroupToolBar {
            id: toolBar
            implicitHeight: 40
            Layout.fillWidth: true
        }

        GridView {
            id: view
            clip: true
            reuseItems: true

            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical: FluScrollBar {
            }

            //height: view.contentHeight
            delegate: ProxyItem {
                //width: view.cellWidth
                //height: view.cellHeight
                width: 300
                height: 70
                name: nameRole
                delay: delayRole
                udp: udpRole
                type: typeRole
                checked: model.index === view.currentIndex

                onSelectProxyClicked: {
                    dataModel.currentIndex = model.row;
                }
                onDelayDetectClicked: {
                    dataModel.delayDetect(index);
                }
                Behavior on opacity {
                    NumberAnimation {
                        duration: 500
                    }
                }
            }
            displaced: Transition {
                NumberAnimation {
                    properties: "x,y"
                    easing.type: Easing.OutQuad
                }
            }
            cellWidth: 300 // 设置每个元素的宽度
            cellHeight: 70 // 设置每个元素的高度
            currentIndex: dataModel ? dataModel.currentIndex : -1
            //implicitHeight: contentHeight
            //interactive: false
        }
    }
}
