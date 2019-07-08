#include "portmodel.h"

PortModel::PortModel() : QAbstractListModel(nullptr), m_index(0) {
    qDebug() << "Empty constructor called. Everything will fail!";
}

PortModel::PortModel(pa_context* context, const pa_sink_info* info, QObject* parent)
    : QAbstractListModel(parent),
      m_context(context), m_sinkInfo(info), m_index(info->index) {
    qDebug() << info->n_ports << " ports found";
    for (size_t i = 0; i < info->n_ports; i++) {
        this->ports.append(Port(info->ports[i], i, this));
        if (info->ports[i] == info->active_port) {
            this->m_activeIndex = i;
            this->ports[i].setAvailable(true);
        }
    }
}

PortModel::PortModel(const PortModel &other) : QAbstractListModel(other.parent()), m_index(other.m_index) {
    this->ports = QList<Port>(other.ports);
    this->m_context = other.m_context;
    this->m_activeIndex = other.m_activeIndex;
    this->m_sinkInfo = other.m_sinkInfo;
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
        return this->m_activeIndex == index.row();
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

void PortModel::paSinkInfoCallback(pa_context* c, const pa_sink_info* info, int eol, void* userdata) {
    PortModel* self = reinterpret_cast<PortModel*>(userdata);
    if (eol) {
        return;
    } else {
        //TODO: handle other updates related to the sink
        for (size_t i = 0; i < info->n_ports; i++) {
            if (info->ports[i] == info->active_port) {
                int oldIndex = static_cast<int>(self->m_activeIndex);
                int newIndex = static_cast<int>(i);
                self->m_activeIndex = i;
                self->dataChanged(self->createIndex(oldIndex, 0), self->createIndex(oldIndex, 0));
                self->dataChanged(self->createIndex(newIndex, 0), self->createIndex(newIndex, 0));
            }
        }
    }
}

void PortModel::setActivePort(QString name) {
    pa_context_set_sink_port_by_index(this->m_context, this->m_index, name.toLatin1().constData(), nullptr, nullptr);
}

void PortModel::update() {
    pa_context_get_sink_info_by_index(this->m_context, this->m_index, PortModel::paSinkInfoCallback, this);
}

int PortModel::findPortIndex(pa_sink_port_info* lostPort) {
    auto it = std::find_if(ports.begin(), ports.end(), [lostPort](Port const& port)->bool {
        return port.rawInfo() == lostPort;
    });
    if (it == ports.end()) {
        return -1;
    }
    return std::distance(ports.begin(), it);
}

