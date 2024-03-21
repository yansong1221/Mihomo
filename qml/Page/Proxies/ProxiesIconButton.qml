import FluentUI
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../../Common"

IconButton {

    property var darkIcon
    property var lightIcon
    property var tooltip
    height: 32
    width: 32

    source: FluTheme.dark ? darkIcon : lightIcon
    FluTooltip {
        visible: parent.hovered
        text: tooltip
        delay: 1000
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            cursorShape = Qt.PointingHandCursor;
        }

        onExited: {
            cursorShape = Qt.ArrowCursor;
        }
    }
}
