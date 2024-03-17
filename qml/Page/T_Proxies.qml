import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import "./Proxies"
import Clash.Meta

FluContentPage {
    id: root1
    title: qsTr("Proxies")

    Component.onCompleted:
    //proxiesModel.reload()
    {
    }
    ProxiesGroupItemModel {
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

        GridView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip:true
            //anchors.fill: parent
            delegate: listDelegate
            model: proxiesModel
            cellWidth: 300 // 设置每个元素的宽度
            cellHeight: 70 // 设置每个元素的高度
            currentIndex: proxiesModel.currentIndex
        }
        Component {
            id: listDelegate

            ProxyItem {
                width: listView.cellWidth // 使用GridView的cellWidth属性
                height: listView.cellHeight // 使用GridView的cellHeight属性
                name: model.display.name
                type: "222"
                checked: model.index === listView.currentIndex

                onSelectProxyClicked: {
                    console.log("1111111");
                    //listView.currentIndex = index
                    proxiesModel.currentIndex = model.row;
                    console.log("Type of myValue:", typeof model);
                    // 获取对象的所有属性列表
                    var properties = Object.keys(model);

                    // 遍历属性列表并打印属性名和属性值
                    for (var i = 0; i < properties.length; ++i) {
                        var propertyName = properties[i];
                        var propertyValue = model[propertyName];
                        console.log(propertyName + ": " + propertyValue);
                    }
                }
                onDelayDetectClicked:{
                    console.log("2222222222");
                }
            }
        }
    }
}
