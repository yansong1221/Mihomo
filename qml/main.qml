import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1
import QtQuick.Layouts
import QtQml
import "./ControlBar"
import "./WindowBar"
import Clash.Meta
import FluentUI

ClashWindow {
    id: window
    stayTop: false
    width: 800
    height: 600
    color: window.active ? FluTheme.windowActiveBackgroundColor : FluTheme.windowBackgroundColor
    title: qsTr("Hello, world!")
    Component.onCompleted: {
        //FluTheme.dark = true;
        window.visible = true
        ClashMetaProcess.restart()
    }

    WindowBar {
        id: titleBar
        window: window
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 32
    }
    ControlBar {
        id: controlBar
        anchors {
            top: titleBar.bottom
            topMargin: 1
            left: parent.left
            bottom: parent.bottom
        }
        width: parent.width
    }
}
