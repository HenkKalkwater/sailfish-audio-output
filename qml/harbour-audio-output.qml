import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import me.henkkalkwater 1.0
import "pages"

ApplicationWindow
{
	id: root
	initialPage: Component { SelectPage { } }
	cover: Qt.resolvedUrl("cover/CoverPage.qml")
	allowedOrientations: defaultAllowedOrientations
	signal qmlSignal(string sink, string port)

	Connections {
		target: Ports
		onError: function(message) {
			portError.summary = qsTr("Error: %s", message)
			portError.urgency = Critical
			portError.publish()
		}
	}

	Notification {
		id: portError
		summary: ""
	}
}
