import QtQuick 2.15
import QtQuick.Controls 2.15
import FluentUI

FluNavigationView {
    id:navigationView
    z: 999
    autoSuggestBox: FluAutoSuggestBox {
        iconSource: FluentIcons.Search
        items: ItemsOriginal.getSearchData()
        placeholderText: qsTr("Search")
        onItemClicked: data => {
            ItemsOriginal.startPageByItem(data);
        }
    }
    items: FluObject {
        FluPaneItem {
            title: qsTr("Home")
            icon: FluentIcons.Home
            url:"/Clash/Meta/GUI/qml/Page/T_Home.qml"
            onTap: {
                navigationView.push(url)
            }
        }
        FluPaneItem {
            title: qsTr("Proxies")
            icon: FluentIcons.Home
            url:"/Clash/Meta/GUI/qml/Page/T_Proxies.qml"
            onTap: {
                navigationView.push(url)
            }
        }
    }
}
