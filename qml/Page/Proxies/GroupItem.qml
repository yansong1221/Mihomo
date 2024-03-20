import FluentUI
import QtQuick
import QtQuick.Layouts

FluExpander {
    id: root
    property var model
    contentHeight: expand ? view.contentHeight : 1
    //expand: false

    GridView {
        id: view
        clip: true

        //flickable: false

        anchors.fill: parent
        height: root.expand ? view.contentHeight : 1
        delegate: ProxyItem {
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
                root.model.currentIndex = model.row;
            }
            onDelayDetectClicked: {
                display.delayDetect();
            }
        }
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
}
