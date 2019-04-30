#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <pulse/pulseaudio.h>

#include "portmodel.h"

class Listener: public QObject
{
	Q_OBJECT
public:
    explicit Listener( QObject* parent = 0) : QObject(parent){
    }
	~Listener() {}
    void initContext();
    pa_context* getPaContext() { return context; }
public slots:
    void changeOutput(const QString &sink, const QString &port);
    void onAboutQuit();
private:
    pa_context* context = nullptr;
};

#endif // AUDIOOUTPUT_H
