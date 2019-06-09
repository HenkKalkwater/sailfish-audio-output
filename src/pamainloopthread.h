#ifndef PAEVENTLOOPTHREAD_H
#define PAEVENTLOOPTHREAD_H

#include <pulse/pulseaudio.h>
#include <QThread>
#include <QDebug>

class PAMainloopThread : public QThread {
public:
    PAMainloopThread(pa_mainloop* mainloop);
    void stop();
protected:
    void run() override;
    pa_mainloop* mainloop;
};

#endif // PAEVENTLOOPTHREAD_H
