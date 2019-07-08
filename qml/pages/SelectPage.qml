import QtQuick 2.6
import Sailfish.Silica 1.0
import me.henkkalkwater 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: outputList
        contentHeight: pageContent.height
        anchors.fill: parent
        VerticalScrollDecorator {}
        Column {
            id: pageContent
            PageHeader { title: qsTr("Select audio output")}

            Repeater {
                /*:Header of the page */
                model: Sinks
                delegate: Column {
                    id: sinkList
                    width: parent.width
                    property var ports: Sinks.getPortModel(model.index)
                    SectionHeader {
                        text: model.description
                    }
                    // This label is visible if no ports were found
                    Label {
                        visible: portList.count === 0
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: Theme.horizontalPageMargin
                        anchors.rightMargin: Theme.horizontalPageMargin
                        width: parent.width
                        wrapMode: Text.Wrap
                        color: Theme.secondaryHighlightColor
                        text: qsTr("No ports were found for this sink")
                    }
                    Repeater {
                        id: portList
                        model: ports
                        delegate: BackgroundItem {
                            id: delegate
                            height: col.height
                            width: parent.width
                            enabled: !model.active
                            Column {
                                id: col
                                anchors.left: parent.left
                                anchors.leftMargin: Theme.horizontalPageMargin
                                anchors.right: parent.right
                                anchors.rightMargin: Theme.horizontalPageMargin
                                topPadding: Theme.paddingMedium
                                bottomPadding: Theme.paddingMedium
                                spacing: Theme.paddingSmall

                                Label {
                                    text: model.description
                                    color: model.active ?  Theme.highlightColor : Theme.primaryColor
                                    elide: Text.ElideRight
                                    width: parent.width
                                }

                                Label {
                                    /*: Available refers to a certain port being available */
                                    /*: Unavailable refers to a cetrain port not being available */
                                    text: (model.available ? qsTr("Available") : qsTr("Not available")) +
                                          /*: Priority has to do something with the preference of port */
                                          " | " + qsTr("Priority: %1").arg(model.priority) +
                                          " | " + model.name
                                    color: Theme.secondaryColor
                                    font.pixelSize: Theme.fontSizeSmall
                                    elide: Text.ElideRight
                                    width: parent.width
                                }
                            }
                            onClicked: {
                                ports.setActivePort(model.name)
                                Ports.update()
                            }
                        }
                    }
                }
            }
            ViewPlaceholder {
                enabled: outputList.count === 0;
                /*: Shown when no ports/sinks could be found */
                text: qsTr("Couldn't find any audio outputs")
            }
        }
    }

}
