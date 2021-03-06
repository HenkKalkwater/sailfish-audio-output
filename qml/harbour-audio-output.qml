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

	Connections {
        target: Sinks
		onError: function(message) {
			console.log("Error: ", message)
			/*: Notification shown when the command line tool gives an error */
			portError.summary = qsTr("Error: %1").arg(message)
			portError.urgency = Critical
			portError.publish()
		}
	}

	Notification {
		id: portError
		summary: ""
	}
}
