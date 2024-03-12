import QtQuick
import QtQuick.Window
import QWindowKit 1.0

Rectangle {
    id: titleBar
    property var window: null
    color: window && window.active ? "#3C3C3C" : "#505050"

    function setup(w) {
        titleBar.window = w;
        windowAgent.setup(w);
        windowAgent.setTitleBar(titleBar);
        w.visible = true;
    }
    WindowAgent {
        id: windowAgent
    }

    Image {
        id: iconButton
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 10
        }
        width: 18
        height: 18
        mipmap: true
        source: "qrc:///images/Meta.png"
    }

    Text {
        anchors {
            verticalCenter: parent.verticalCenter
            left: iconButton.right
            leftMargin: 10
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        color: "#ECECEC"
        text: window ? window.title : ""
    }

    Row {
        anchors {
            top: parent.top
            right: parent.right
        }
        height: parent.height

        QWKButton {
            id: minButton
            height: parent.height
            source: "qrc:///icons/dark/windowbar/min-button.svg"
            onClicked: window.showMinimized()
            Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Minimize, minButton)
        }

        QWKButton {
            id: maxButton
            height: parent.height
            source: {
                if (window.visibility === Window.Maximized)
                    return "qrc:///icons/dark/windowbar/normal-button.svg";
                else {
                    return "qrc:///icons/dark/windowbar/max-button.svg";
                }
            }
            onClicked: {
                if (window.visibility === Window.Maximized) {
                    window.showNormal();
                } else {
                    window.showMaximized();
                }
            }
            Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Maximize, maxButton)
        }

        QWKButton {
            id: closeButton
            height: parent.height
            source: "qrc:///icons/dark/windowbar/close-button.svg"
            background: Rectangle {
                color: {
                    if (!closeButton.enabled) {
                        return "gray";
                    }
                    if (closeButton.pressed) {
                        return "#e81123";
                    }
                    if (closeButton.hovered) {
                        return "#e81123";
                    }
                    return "transparent";
                }
            }
            onClicked: window.close()
            Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Close, closeButton)
        }
    }
}
