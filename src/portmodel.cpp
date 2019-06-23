#include "portmodel.h"

PortModel::PortModel(const PortModel &other) : QAbstractListModel(other.parent()), m_index(other.m_index) {
    this->ports = QList<Port>(other.ports);
    this->m_activePort = other.m_activePort;
    //this->ports = other.ports;
    //this->port_count = other.port_count;
}

PortModel::PortModel(int index, pa_sink_port_info** info, size_t info_length, pa_sink_port_info* activePort,
                     QObject *parent)
    : QAbstractListModel(parent), m_index(index), m_activePort(activePort) {
    qDebug() << info_length << " ports found";
    for (size_t i = 0; i < info_length; i++) {
        this->ports.append(Port(info[i], i));
    }
}

QVariant PortModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
    return QVariant();
    }
    switch (role) {
    case NAME:
        qDebug() << "Returning name of index " << index.row() << ": " << ports[index.row()].name();
        return ports[index.row()].name();
    case DESCRIPTION:
        return ports[index.row()].description();
    case AVAILABLE:
        return ports[index.row()].available();
    case PRIORITY:
        return ports[index.row()].priority();
    case ACTIVE:
        return this->m_activePort == ports[index.row()].rawInfo();
    default:
        return QVariant();
    }
}

int PortModel::rowCount(const QModelIndex &parent) const {
    return this->ports.size();
}

QHash<int, QByteArray> PortModel::roleNames() const {
	QHash<int, QByteArray> result;
    //result[SINK] = "sink";
    //result[SINK_DESCRIPTION] = "sinkDescription";
    result[NAME] = "name";
    result[DESCRIPTION] = "description";
    result[AVAILABLE] = "available";
    result[PRIORITY] = "priority";
    result[ACTIVE] = "active";
    return result;
}

