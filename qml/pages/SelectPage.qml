import QtQuick 2.6
import Sailfish.Silica 1.0
import me.henkkalkwater 1.0

Page {
	id: page

	// The effective value will be restricted by ApplicationWindow.allowedOrientations
	allowedOrientations: Orientation.All
	ListModel {
		id: mModel
		ListElement { display: "Speaker"; internal: "output-speaker" }
		ListElement { display: "Wired headset"; internal: "output-wired_headset" }
	}
	Component.onCompleted: Ports.update(true)
	Component {
		id: sectionHeading
		Label {
			anchors.left: parent.left
			anchors.right: parent.right
			//anchors.leftMargin: Theme.horizontalPageMargin
			//anchors.rightMargin: Theme.horizontalPageMargin
			padding: Theme.paddingLarge
			text: section
			color: Theme.highlightColor
			horizontalAlignment: "AlignRight"
		}
	}

	SilicaListView {
		//visible: Ports.rowCount() > 0
		//visible: false
		id: outputList
		PullDownMenu {
			quickSelect: true
			MenuItem {
				text: qsTr("Refresh")
				onClicked: Ports.update(true)
			}
		}
		anchors.fill: parent
		/*:Header of the page */
		header: PageHeader { title: qsTr("Select audio output")}
		model: Ports
		section.property: "sinkDescription"
		section.delegate: sectionHeading

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
					text: model.longName
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
						 " | " + model.shortName
					color: Theme.secondaryColor
					font.pixelSize: Theme.fontSizeSmall
					elide: Text.ElideRight
					width: parent.width
				}
			}
			onClicked: {
				root.qmlSignal(model.sink, model.shortName)
				Ports.update()
			}
		}
		ViewPlaceholder {
			enabled: outputList.count === 0;
			/*: Shown when no ports/sinks could be found */
			text: qsTr("Couldn't find any audio outputs")
		}
	}
	Timer {
		interval: 5000
		repeat: true
		running: true
		onTriggered: Ports.update(false)
	}


}
