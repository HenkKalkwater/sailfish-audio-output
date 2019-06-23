#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <pulse/pulseaudio.h>

#include "portmodel.h"
#include "sinkmodel.h"
#include "pamainloopthread.h"

class Listener: public QObject
{
	Q_OBJECT
public:
    explicit Listener( QObject* parent = 0) : QObject(parent){
    }
	~Listener() {}
    /**
     * @brief init Initialises the connection to the PulseAudio server
     * @note Very much inspired by Godot's PulseAudio code
     */
    void init();

    /**
     * @brief getPaContext
     * @return the PulseAudio context used by this application
     */
    pa_context* getPaContext() { return context; }

    /**
     * @brief stateCallback Callback for PulseAudio's state events
     */
    static void stateCallback(pa_context* c, void* userdata);

    /**
     * @brief eventCallback Callback that gets called whenever the state of PulseAudio changes.
     * @param c
     * @param t
     * @param idx
     * @param userdata
     */
    static void eventCallback(pa_context* c, pa_subscription_event_type_t t, uint32_t idx, void *userdata);

    /**
     * @brief successCallback Callback to determine if the action PulseAudio executed was successful
     * @param c
     * @param success
     * @param userdata
     */
    static void successCallback(pa_context* c, int success, void* userdata);
    int paReady = 0;
public slots:
    void changeOutput(const QString &sink, const QString &port);
    void onAboutQuit();
private:
    /**
     * @brief mainloop The mainloop of the PulseAudio
     * @see PulseAudio's documentation for more details.
     */
    pa_mainloop* mainloop = nullptr;
    pa_context* context = nullptr;
    PAMainloopThread* mainloopThread = nullptr;

};

#endif // AUDIOOUTPUT_H
