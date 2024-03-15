import QtQuick
import QtQuick.Layouts
import FluentUI

FluToggleButton {
    id: control
    property var darkSource
    property var lightSource
    implicitWidth:120
    implicitHeight:40
    contentItem: RowLayout {
        anchors.centerIn: control
        //Layout.fillWidth: true
        //Layout.fillHeight: true
        //Layout.alignment:Qt.AlignHCenter
        spacing:10
        FluText {
            text: control.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: {
                if (checked) {
                    if (FluTheme.dark) {
                        if (!enabled) {
                            return Qt.rgba(173 / 255, 173 / 255, 173 / 255, 1);
                        }
                        return Qt.rgba(0, 0, 0, 1);
                    } else {
                        return Qt.rgba(1, 1, 1, 1);
                    }
                } else {
                    if (FluTheme.dark) {
                        if (!enabled) {
                            return Qt.rgba(131 / 255, 131 / 255, 131 / 255, 1);
                        }
                        if (!checked) {
                            if (pressed) {
                                return Qt.rgba(162 / 255, 162 / 255, 162 / 255, 1);
                            }
                        }
                        return Qt.rgba(1, 1, 1, 1);
                    } else {
                        if (!enabled) {
                            return Qt.rgba(160 / 255, 160 / 255, 160 / 255, 1);
                        }
                        if (!checked) {
                            if (pressed) {
                                return Qt.rgba(96 / 255, 96 / 255, 96 / 255, 1);
                            }
                        }
                        return Qt.rgba(0, 0, 0, 1);
                    }
                }
            }
        }
        Image {
            source: {
                if (checked) {
                    return FluTheme.dark ? control.lightSource : control.darkSource;
                }
                else{
                    return FluTheme.dark ? control.darkSource : control.lightSource;
                }
            }
            Layout.maximumWidth: 16
            Layout.maximumHeight: 16
            mipmap: true
        }
    }
}
