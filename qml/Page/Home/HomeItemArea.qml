import QtQuick
import QtQuick.Layouts
import FluentUI

FluArea {
    Layout.fillWidth: true
    height: 50
    property FluObject leftItems
    property FluObject rightItems
    Component.onCompleted: {
        if (leftItems) {
            for (var i = 0; i < leftItems.children.length; i++) {
                var item = leftItems.children[i];
                leftLayout.children.push(item);
                
            }
        }
        if (rightItems) {
            for (var i = 0; i < rightItems.children.length; i++) {
                var item = rightItems.children[i];
                rightLayout.children.push(item);
            }
        }
    }
    RowLayout {

        anchors.fill: parent
        anchors.leftMargin: 40
        anchors.rightMargin: 40

        RowLayout {
            id: leftLayout
            Layout.alignment: Qt.AlignLeft
            spacing: 10
        }
        RowLayout {
            id: rightLayout
            Layout.alignment: Qt.AlignRight
            spacing: 10
        }
    }
}
