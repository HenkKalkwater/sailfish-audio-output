#include "sink.h"

Sink::Sink(const Sink& other) : Sink(other.sink, other.parent()){

}

Sink::Sink(const pa_sink_info* sink, QObject *parent) : QObject(parent),
    sink(sink){

    if (sink == nullptr) return;

    this->m_name = QString(sink->name);
    if (sink->description != nullptr) {
        this->m_description = QString(sink->description);
    }
}

void Sink::setVolume(float newVolume) {
    //TODO: implement
}
