import QtQuick 2.0
import Sailfish.Silica 1.0
import me.henkkalkwater 1.0

CoverBackground {
	id: coverBackground

    CoverPlaceholder {
        text: qsTr("Audio Output")
        icon.source: "./cover-bg.png"
    }
}
