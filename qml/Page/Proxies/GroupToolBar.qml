import FluentUI
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    signal locateClicked
    signal delayDetectClicked

    Row {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        ProxiesIconButton {
            darkIcon: "qrc:/assets/icons/dark/location.svg"
            lightIcon: "qrc:/assets/icons/light/location.svg"
            tooltip: qsTr("Save the active project")
            onClicked: root.locateClicked()
        }
        ProxiesIconButton {
            darkIcon: "qrc:/assets/icons/dark/delay.svg"
            lightIcon: "qrc:/assets/icons/light/delay.svg"
            tooltip: qsTr("Save the active project")
            onClicked: root.delayDetectClicked()
        }
    }
}
