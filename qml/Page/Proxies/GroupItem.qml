import FluentUI
import QtQuick
import QtQuick.Layouts

/*
FluExpander {
    id: root
    property var model
    contentHeight: view.contentHeight

    onExpandChanged:{
        console.log("!111111111")
        contentHeight = view.contentHeight
    }
    GridView {
        id: view
        clip: true
        //flickable: false
        anchors.fill: parent
        height: view.contentHeight
        delegate: proxyItemDelegate
        model: root.model
        cellWidth: 300 // 设置每个元素的宽度
        cellHeight: 70 // 设置每个元素的高度
        currentIndex: root.model.currentIndex
        //implicitHeight: contentHeight
        interactive: false
    }
    Component.onCompleted: {
        headerText = model.groupName;
        view.model = model;
    }

    Component {
        id: proxyItemDelegate

        ProxyItem {
            width: view.cellWidth // 使用GridView的cellWidth属性
            height: view.cellHeight // 使用GridView的cellHeight属性
            name: display.name
            delay: display.delay

            checked: model.index === view.currentIndex

            Component.onCompleted: {
                udp = display.udp;
                type = display.type;
            }

            onSelectProxyClicked: {
                console.log("1111111");
                root.model.currentIndex = model.row;
            }
            onDelayDetectClicked: {
                display.delayDetect();
            }
        }
    }
}
*/

Item {
    id: root
    property var model
    property bool expand: true
    //height: view.visible ? view.height : 0 + title.implicitHeight
    //contentHeight: view.height
    implicitHeight: expand ? view.contentHeight + title.implicitHeight : title.implicitHeight
    clip: true
    Component.onCompleted: {
        //headerText = model.groupName;
        view.model = model;
    }

    Rectangle {
        id: title
        clip: true
        width: parent.width
        implicitHeight: 45

        MouseArea {

            anchors.fill: parent
            onClicked: {
                expand = !expand;
            }
        }
        color: "red"
    }
    GridView {
        id: view

        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        height: contentHeight

        visible: root.expand

        clip: true
        //flickable: false
        //anchors.fill: parent
        //height: 100
        delegate: proxyItemDelegate
        highlight: Item {
            z: view.z + 2
            Rectangle {
                anchors.fill: parent
                //anchors.leftMargin: tabListView.itemSapcing

                color: FluTheme.itemPressColor
                radius: tabListView.itemRadius
            }
        }
        //model: root.model
        cellWidth: 300 // 设置每个元素的宽度
        cellHeight: 70 // 设置每个元素的高度
        currentIndex: root.model.currentIndex
        //implicitHeight: contentHeight
        interactive: false
    }

    Component {
        id: proxyItemDelegate

        ProxyItem {
            width: view.cellWidth // 使用GridView的cellWidth属性
            height: view.cellHeight // 使用GridView的cellHeight属性
            name: display.name
            delay: display.delay

            checked: model.index === view.currentIndex

            Component.onCompleted: {
                udp = display.udp;
                type = display.type;
            }

            onSelectProxyClicked: {
                console.log("1111111");
                root.model.currentIndex = model.row;
            }
            onDelayDetectClicked: {
                display.delayDetect();
            }
        }
    }
}
