import QtQuick
import QtQuick.Window
import QWindowKit 1.0
import QtQuick.Controls 6.2

Rectangle {
    id: root
    property var window: null
    color: window && window.active ? "#3C3C3C" : "#505050"
    property bool isOSX : Qt.platform.os === "osx"
    
    function setup(w) {
        window = w;
        windowAgent.setup(w);
        windowAgent.setTitleBar(root);
        windowAgent.setHitTestVisible(stayTopButton);
        windowAgent.setSystemButtonArea(macSystemButton)
        w.visible = true;
        w.stayTop = true
    
    }
    
    Component.onCompleted: {
        console.log("Current platform: " + Qt.platform.os);
    }
    
    WindowAgent {
        id: windowAgent
    }

    Row{
        anchors {
            top: parent.top
            left: parent.left
        }
        height: parent.height

        Item {
        id: macSystemButton
        visible: isOSX
        height: parent.height
        width: 80
        //color: "red"

        }
        Image {
        id: iconButton
        anchors {
            verticalCenter: parent.verticalCenter
            leftMargin: 10
        }
        width: 18
        height: 18
        mipmap: true
        source: "qrc:/assets/meta.png"
        }
        Text {
        anchors {
            verticalCenter: parent.verticalCenter
            leftMargin: 10
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        color: "#ECECEC"
        text: window ? window.title : ""
    }
    }
    

    

    Row {
        anchors {
            top: parent.top
            right: parent.right
        }
        height: parent.height

        QWKButton {
            id: stayTopButton
            checkable: true
            height: parent.height
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
                window.stayTop = checked;
            }
        }
        QWKButton {
            id: minButton
            height: parent.height
            source: "qrc:/assets/icons/dark/windowbar-mix.svg"
            onClicked: window.showMinimized()
            visible: !isOSX
            Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Minimize, minButton)
        }

        QWKButton {
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

        QWKButton {
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
