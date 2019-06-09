#include "pamainloopthread.h"

PAMainloopThread::PAMainloopThread(pa_mainloop* mainloop) {
    this->mainloop = mainloop;
}

void PAMainloopThread::run() {
    qDebug() << "Started PulseAudio mainloop";
    pa_mainloop_run(mainloop, nullptr);
}

void PAMainloopThread::stop() {
    pa_mainloop_quit(mainloop, 0);
    qDebug() << "Stopped PulseAudio mainloop";
}
