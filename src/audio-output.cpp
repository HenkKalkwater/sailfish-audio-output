#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QtQuick>
#include <sailfishapp.h>
#include <QString>
#include <QQuickView>
#include <QGuiApplication>
#include <QObject>
#include <QQmlEngine>
#include "audio-output.h"

int main(int argc, char *argv[]) {
	// SailfishApp::main() will display "qml/audio-output.qml", if you need more
	// control over initialization, you can use:
	//
	//   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
	//   - SailfishApp::createView() to get a new QQuickView * instance
	//   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
	//   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
	//
	// To display the view, call "show()" (will show fullscreen on device).
	Listener listener;
	QGuiApplication* app = SailfishApp::application(argc, argv);
	QQuickView* view = SailfishApp::createView();
	qmlRegisterSingletonType<PortModel>("me.henkkalkwater", 1, 0, "Ports", [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		PortModel* portModel = new PortModel();
		return portModel;
	});
	view->setSource(SailfishApp::pathToMainQml());
	view->show();
	QObject::connect(view->rootObject(), SIGNAL(qmlSignal(QString, QString)),
					 &listener, SLOT(changeOutput(QString, QString)));
	return app->exec();
}
