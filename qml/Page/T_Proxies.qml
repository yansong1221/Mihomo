import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import "./Proxies"
import Clash.Meta

FluContentPage {
    id: root1
    //clip: true
    title: qsTr("Proxies")

    ProxiesGroupModel {
        id: proxiesModel
        Component.onCompleted: {
            reload();
        }
        onReloadSuccessed: {
            proxiesGlobalModelFilter.dataModel = proxiesGlobalModelFilter.rowCount() === 0 ? null : proxiesGlobalModelFilter.data(proxiesGlobalModelFilter.index(0, 0));
        }
    }
    ProxiesGroupModelFilter {
        id: proxiesRuleModelFilter
        sourceModel: proxiesModel
        filerMode: "rule"
    }
    ProxiesGroupModelFilter {
        id: proxiesGlobalModelFilter
        sourceModel: proxiesModel
        filerMode: "global"

        property var dataModel
    }

    Component {
        id: global_group_comp
        FluPage {
            clip: true
            GroupItem {
                dataModel: proxiesGlobalModelFilter.dataModel
                anchors.fill: parent
            }
        }
    }
    Component {
        id: rule_group_comp
        FluPage {
            clip: true
            RuleModePage {
                anchors.fill: parent
                dataModel: proxiesRuleModelFilter
            }
        }
    }
    Component {
        id: direct_group_comp
        FluPage {
            clip: true
            DirectModePage {
                anchors.fill: parent
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            implicitHeight: 80

            ProxyModeSelector {
                id: modeListView
                anchors.centerIn: parent
                width: itemWidth * model.count
                height: 30
                clip: true
                highlightMoveDuration: 200
            }
        }
        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            sourceComponent: {
                if (modeListView.currentIndex == 0)
                    return global_group_comp;
                else if (modeListView.currentIndex == 1)
                    return rule_group_comp;
                return direct_group_comp;
            }
        }
        // StackLayout {
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        //     currentIndex: modeListView.currentIndex
        //     FluPage {
        //         visible: false
        //         clip: true
        //         GroupItem {
        //             id: globalGroupItem
        //             anchors.fill: parent
        //         }
        //     }
        //     FluPage {
        //         clip: true
        //         RuleModePage {
        //             anchors.fill: parent
        //             dataModel: proxiesRuleModelFilter
        //         }
        //     }
        //     FluPage {
        //         clip: true
        //         DirectModePage {
        //             anchors.fill: parent
        //         }
        //     }
        // }
        // SwipeView {
        //     id: view
        //     interactive: false
        //     currentIndex: modeListView.currentIndex
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        //     GroupItem {
        //         id: globalGroupItem
        //     }
        //     RuleModePage {
        //         dataModel: proxiesRuleModelFilter
        //     }
        //     DirectModePage {
        //     }
        // }
    }
}
