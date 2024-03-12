import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1
import QtQml
import QWindowKit 1.0

Window {
    id: window

    width: 800
    height: 600
    color: darkStyle.windowBackgroundColor
    title: qsTr("Hello, world!")
    Component.onCompleted: {
        titleBar.setup(window);
    }

    TitleBar {
        id: titleBar
        anchors {
            top: parent.top
            topMargin: 1
            left: parent.left
            right: parent.right
        }

        height: 32
    }
    Timer {
        interval: 100
        running: true
        repeat: true
        onTriggered: timeLabel.text = Qt.formatTime(new Date(), "hh:mm:ss")
    }

    QtObject {
        id: lightStyle
    }

    QtObject {
        id: darkStyle
        readonly property color windowBackgroundColor: "#1E1E1E"
    }

    Label {
        id: timeLabel
        anchors.centerIn: parent
        font {
            pointSize: 75
            bold: true
        }
        color: "#FEFEFE"
    }
}