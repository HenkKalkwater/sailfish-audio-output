#include "sinkmodel.h"

SinkModel::SinkModel(pa_context *context, QObject* parent)
    : QAbstractListModel(parent),
    context(context){
    refresh();
}

void SinkModel::paSinkInfoCallback(pa_context* c, const pa_sink_info* info, int eol, void* userdata) {
    SinkModel* self = reinterpret_cast<SinkModel*>(userdata);

    if (eol) {
        self->refreshMutex.lock();
        self->doneRefreshing = true;
        self->refreshing.wakeAll();
        self->refreshMutex.unlock();
    } else {
        self->sinks.append(Sink(info));
        self->portModels.append(PortModel(info->index, info->ports, info->n_ports, info->active_port));
    }
}

QVariant SinkModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    switch(role) {
    case INDEX:
        return sinks[index.row()].index();
    case NAME:
        return sinks[index.row()].name();
    case DESCRIPTION:
        return sinks[index.row()].description();
    default:
        return QVariant();
    }
}

void SinkModel::refresh() {
    qDebug() << "Refreshing";
    this->beginResetModel();
    this->doneRefreshing = false;
    pa_context_get_sink_info_list(this->context, SinkModel::paSinkInfoCallback, this);
    qDebug() << "Requested sink list";

    // Wait until the callbacks are done.
    if (!this->doneRefreshing) {
        qDebug() << "Waiting...";
        this->refreshing.wait(&this->refreshMutex);
    }

    this->endResetModel();
}

PortModel* SinkModel::getPortModel(int index) {
    auto it = std::find_if(this->sinks.begin(), this->sinks.end(),
        [index](Sink const& sink)->bool {
           return sink.index() == index;
    });
    if (it != this->sinks.end()) {
        qDebug() << "Found PortModel for index " << index;
        return &portModels[std::distance(this->sinks.begin(), it)];
    } else {
        qDebug() << "Didn't find PortModel for index " << index;
        return nullptr;
    }
}

int SinkModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return this->sinks.size();
}

QHash<int, QByteArray> SinkModel::roleNames() const{
    QHash<int, QByteArray> names;
    names[INDEX] = "index";
    names[NAME] = "name";
    names[DESCRIPTION] = "description";

    return names;
}

