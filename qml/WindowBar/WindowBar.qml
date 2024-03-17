import QtQuick
import QtQuick.Window
import QWindowKit 1.0
import QtQuick.Controls 6.2
import FluentUI

Rectangle {
    id: root
    property var window
    color:"transparent"
    //color: window && window.active ? "#3C3C3C" : "#505050"
    readonly property bool isOSX: Qt.platform.os === "osx"

    Component.onCompleted: {
        console.log("Current platform: " + Qt.platform.os);

        windowAgent.setup(root.window);
        windowAgent.setTitleBar(root);
        windowAgent.setHitTestVisible(stayTopButton);
        if (root.isOSX)
            windowAgent.setSystemButtonArea(macSystemButton);
        //root.window.stayTop = true;
    }

    WindowAgent {
        id: windowAgent
    }

    Item {
        id: macSystemButton
        height: parent.height
        width: 80
        anchors {
            top: parent.top
            left: parent.left
        }
        visible: root.isOSX
    }

    Image {
        id: iconButton
        anchors {
            left: root.isOSX ? undefined : parent.left
            right: root.isOSX ? parent.right : undefined
            verticalCenter: parent.verticalCenter
            leftMargin: root.isOSX ? undefined : 10
            rightMargin: root.isOSX ? 10 : undefined
        }
        width: 18
        height: 18
        mipmap: true
        source: "qrc:/assets/meta.png"
    }

    FluText {
        id: windowTitle
        anchors {
            centerIn : parent
            //verticalCenter: parent.verticalCenter
            leftMargin: 10
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: window ? window.title : ""
    }

    WindowButton {
        id: stayTopButton
        checkable: true
        height: parent.height
        anchors {
            top: parent.top
            right: root.isOSX ? iconButton.left : defaultBar.left
        }
        source: {
            if (stayTopButton.checked) {
                return "qrc:/assets/icons/dark/stays-on-top-checked.svg";
            } else {
                return "qrc:/assets/icons/dark/stays-on-top.svg";
            }
        }
        onCheckedChanged: {
            if (!window)
                return;
            FluTheme.darkMode = checked
            window.stayTop = checked;
        }
    }

    Row {
        id: defaultBar
        visible: !root.isOSX
        anchors {
            top: parent.top
            right: parent.right
        }
        height: parent.height
        WindowButton {
            id: minButton
            height: parent.height
            source: "qrc:/assets/icons/dark/windowbar-mix.svg"
            onClicked: window.showMinimized()
            visible: !isOSX
            Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Minimize, minButton)
        }

        WindowButton {
            id: maxButton
            height: parent.height
            visible: !isOSX
            source: {
                if (window.visibility === Window.Maximized)
                    return "qrc:/assets/icons/dark/windowbar-restore.svg";
                else {
                    return "qrc:/assets/icons/dark/windowbar-max.svg";
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

        WindowButton {
            id: closeButton
            height: parent.height
            visible: !isOSX
            source: "qrc:/assets/icons/dark/windowbar-close.svg"
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
