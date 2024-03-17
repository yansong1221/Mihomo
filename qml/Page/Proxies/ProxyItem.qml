import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI

Item {
    id: root
    property int delay: 100
    property bool udp: false
    property string name: "TCPSJDHJ"
    property string type: "TCPSJDHJ111111111111111111\nTCPSJDHJ111111111111111111\nTCPSJDHJ111111111111111111"
    property bool isDelayDetecting: false
    property bool checked: false

    readonly property color secondTextColor: FluTheme.dark ? "#a7a7a7" : "#909090"

    signal delayDetectClicked
    signal selectProxyClicked

    Item {
        anchors.fill: parent
        anchors.margins: 6

        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.selectProxyClicked();
            }
            hoverEnabled: true

            onEntered: {
                cursorShape = Qt.PointingHandCursor;
            }

            onExited: {
                cursorShape = Qt.ArrowCursor;
            }
        }

        RowLayout {
            spacing: 6
            anchors.fill: parent

            Rectangle {
                Layout.fillHeight: true
                implicitWidth: 4
                radius: width / 2
                color: {
                    if (FluTheme.dark) {
                        return root.checked ? "#00aa00" : "#484752";
                    }
                    return root.checked ? "#41b883" : "#dedede";
                }
            }
            Rectangle {
                color: FluTheme.dark ? "#363442" : "#fbfbfd"
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius: 4
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 0

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        ColumnLayout {
                            anchors.verticalCenter: parent.verticalCenter
                            FluText {
                                text: root.name
                            }
                            RowLayout {
                                FluText {

                                    text: root.type
                                    color: root.secondTextColor
                                }
                                Rectangle {
                                    border {
                                        color: root.secondTextColor
                                        width: 1
                                    }
                                    radius: 4
                                    color: "transparent"
                                    width: textUdp.width + 6
                                    height: textUdp.height + 6
                                    FluText {
                                        id: textUdp
                                        text: "UDP"
                                        color: root.secondTextColor
                                        anchors.centerIn: parent
                                    }
                                }
                            }
                        }
                    }
                    FluTextButton {
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignRight
                        implicitWidth: 64
                        text: {
                            if (root.delay === -2)
                                return "";
                            else if (root.delay === -1)
                                return qsTr("timeout");
                            else
                                return root.delay + "ms";
                        }
                        onClicked: {
                            root.delayDetectClicked();
                        }
                    }
                }
            }
        }
    }
}
