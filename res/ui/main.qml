import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ApplicationWindow {
    title: "Feedling RSS Reader"
    visible: true
    width: 800
    height: 600
    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "fetch.png"
            }
            ToolButton {
                iconSource: "edit.png"
            }
            ToolButton {
                iconSource: "add.png"
            }
            Item {
                Layout.fillWidth: true
            }
            ToolButton {
                iconSource: "options.png"
            }
            ToolButton {
                iconSource: "hide.png"
            }
        }
    }  // ToolBar

    SplitView {
        orientation: Qt.Horizontal
        anchors.fill: parent
        TreeView {
            TableViewColumn {
                title: "Name"
                role: "name"
            }

            model: feedsModel
        }

        SplitView {
            orientation: Qt.Vertical
            Layout.fillWidth: true

            ScrollView {
                ListView {
                    delegate: entriesDelegate
                    // model: entriesModel
                    model: ListModel {
                        ListElement {
                            title: "test0"
                        }
                        ListElement {
                            title: "test1"
                        }
                        ListElement {
                            title: "test2"
                        }
                        ListElement {
                            title: "test3"
                        }
                    }

                    Component {
                        id: entriesDelegate
                        Text {
                            text: title
                        }
                    }
                }
            }

            ScrollView {
            }
        }
    }  // GridLayout
}  // ApplicationWindow
