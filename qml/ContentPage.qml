import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "./Page"

Item {

    RowLayout {
        anchors.fill: parent
        Item {
            id: left
            width: 200
            Layout.fillHeight: true
            ListView {
                id: tabListView
                anchors.left: parent.left
                width: 170
                height: parent.height
                clip: true
                highlightMoveDuration: 200

                //interactive: false

                property int itemHeigt: 50
                property int itemSapcing: 3
                property int itemRadius: 4

                orientation: ListView.Vertical
                model: ListModel {
                    ListElement {
                        title: qsTr("主页")
                        content: "Content of Tab 1"
                    }
                    ListElement {
                        title: qsTr("代理")
                        content: "Content of Tab 2"
                    }
                }
                delegate: Item {
                    width: tabListView.width
                    height: tabListView.itemHeigt
                    Rectangle {

                        anchors.fill: parent
                        anchors.topMargin: tabListView.itemSapcing

                        color: FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
                        border.color: FluTheme.dark ? Window.active ? Qt.rgba(55 / 255, 55 / 255, 55 / 255, 1) : Qt.rgba(45 / 255, 45 / 255, 45 / 255, 1) : Qt.rgba(226 / 255, 229 / 255, 234 / 255, 1)

                        radius: tabListView.itemRadius
                        FluText {
                            anchors.centerIn: parent
                            text: model.title
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                tabListView.currentIndex = index;
                            }
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;
                            }

                            onExited: {
                                cursorShape = Qt.ArrowCursor;
                            }
                        }
                    }
                }
                highlight: Item {
                    z: tabListView.z + 2
                    Rectangle {
                        anchors.fill: parent
                        anchors.topMargin: tabListView.itemSapcing

                        color: FluTheme.itemPressColor
                        radius: tabListView.itemRadius
                        Rectangle {
                            width: 3
                            anchors.left: parent.left
                            height: parent.height
                            color: "#0066b4"
                            radius: 2
                        }
                    }
                }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            StackLayout {
                anchors.fill: parent
                currentIndex: tabListView.currentIndex
                T_Home {
                }
                T_Proxies {
                }
            }
        }
    }
}
/*
FluNavigationView {
    id:navigationView
    z: 999
    autoSuggestBox: FluAutoSuggestBox {
        iconSource: FluentIcons.Search
        //items: ItemsOriginal.getSearchData()
        placeholderText: qsTr("Search")
        onItemClicked: data => {
            //ItemsOriginal.startPageByItem(data);
        }
    }
    items: FluObject {
        FluPaneItem {
            title: qsTr("Home")
            icon: FluentIcons.Home
            url:"/Clash/Meta/qml/Page/T_Home.qml"
            onTap: {
                navigationView.push(url)
            }
        }
        FluPaneItem {
            title: qsTr("Proxies")
            icon: FluentIcons.Home
            url:"/Clash/Meta/qml/Page/T_Proxies.qml"
            onTap: {
                navigationView.push(url)
            }
        }
    }
}
*/
