#include "port.h"

Port::Port(const pa_sink_port_info* port, size_t index) :
    m_rawInfo(port), m_name(port->name), m_description(port->description), m_priority(port->priority),
    m_available(port->available), m_index(index){
}

Port::Port(const Port &other) :
    QObject(nullptr),
    m_rawInfo(other.m_rawInfo), m_name(other.m_name), m_description(other.m_description), m_priority(other.m_priority),
    m_available(other.m_available), m_index(other.m_index){
}

void Port::setAvailable(bool isAvailable) {
    this->m_available = isAvailable;
    emit availableChanged();
}
/*Port::operator=(const Port &other) {
    Port port;
    port.m_name = other.m_name;
    return port;
}*/
