#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include <QtQuick>
#include <QString>
#include <QQuickView>
#include <QGuiApplication>
#include <QObject>
#include <QQmlEngine>


#include "audio-output.h"

Listener listener;

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
    //Listener listener;
    listener.init();
    QGuiApplication* app = SailfishApp::application(argc, argv);
	QQuickView* view = SailfishApp::createView();
    qmlRegisterType<PortModel>("me.henkkalkwater", 1, 0, "Ports");
    qmlRegisterSingletonType<SinkModel>("me.henkkalkwater", 1, 0, "Sinks", [&listener](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

        SinkModel* sinkModel = new SinkModel(listener.getPaContext());
        listener.setSinks(sinkModel);
        return sinkModel;
	});
	view->setSource(SailfishApp::pathToMainQml());
	view->show();
    QObject::connect(app, SIGNAL(aboutToQuit()), &listener, SLOT(onAboutQuit()));
	return app->exec();
}

void Listener::init() {
    this->mainloop = pa_mainloop_new();
    qDebug() << "Created PulseAudio mainloop";
    this->context = pa_context_new(pa_mainloop_get_api(mainloop), "Audio Output");
    qDebug() << "Created PulseAudio context";

    pa_context_set_state_callback(this->context, Listener::stateCallback, static_cast<void*>(this));
    pa_context_set_subscribe_callback(this->context, Listener::eventCallback, static_cast<void*>(this));
    //qDebug() << "Setted subscribe callback";


    int result = pa_context_connect(this->context, nullptr, PA_CONTEXT_NOAUTOSPAWN, nullptr);

    if (result < 0) {
        qDebug() << "Connection to the Pulse Audio server failed";
    }
    qDebug() << "Connecting to PulseAudio...";

    // Busywaiting is ugly, but I have no better solution for the time being.
    // Still, we're waiting for localhost to respond, how bad can it be?
    while (this->paReady == 0) {
        pa_mainloop_iterate(this->mainloop, 1, NULL);
    };
    qDebug() << "PulseAudio is ready to roll";

    // Creates a thread that will run the mainloop
    this->mainloopThread = new PAMainloopThread(this->mainloop);
    this->mainloopThread->start();

    pa_context_subscribe(this->context, PA_SUBSCRIPTION_MASK_ALL, nullptr, nullptr);
}

void Listener::stateCallback(pa_context* c, void* userdata) {
    Listener* self = (Listener*) userdata;
    switch(pa_context_get_state(c)) {
    case PA_CONTEXT_TERMINATED: // Explicit fallthrough
    case PA_CONTEXT_FAILED:
        self->paReady = -1;
        qDebug() << "StateCallback: PulseAudio is broken.";
        break;
    case PA_CONTEXT_READY:
        qDebug() << "StateCallback: PulseAudio is ready!";
        self->paReady = 1;
        break;
    default:
        break;
    }
}

void Listener::eventCallback(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata) {
    Q_UNUSED(c)
    Listener* self = reinterpret_cast<Listener*>(userdata);
    qDebug() << "New event callback: type " << t << ", index " << idx;
    if ((t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) == PA_SUBSCRIPTION_EVENT_SINK) {
        if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_CHANGE) {
            qDebug() << "Sink changed!";
            if (self->m_sinks == nullptr) {
                qDebug() << "m_sinks is null?!";
                return;
            }
            self->m_sinks->getPortModel(idx, false)->update();
        }
    }
}

void Listener::onAboutQuit() {
    pa_context_disconnect(context);
    this->mainloopThread->stop();
}

