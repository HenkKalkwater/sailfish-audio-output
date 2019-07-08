#ifndef SINKMODEL_H
#define SINKMODEL_H

#include <QAbstractListModel>
#include <QWaitCondition>
#include <QMutex>
#include <QByteArray>
#include <QHash>
#include <QList>
#include <QThread>
#include <QDebug>

#include <algorithm>
#include <iterator>

#include <pulse/pulseaudio.h>
#include "sink.h"

class SinkModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum SinkRole {
        INDEX = Qt::UserRole + 1,
        NAME,
        DESCRIPTION
    };
    explicit SinkModel(pa_context* context, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex& index, int role) const override;

    /**
     * @brief Refreshes the list of sinks. This reset the model.
     */
    void refresh();

    /**
     * @brief Fetches the PortModel with the ports for the sink with the specified index
     * @param index The index of the sink
     * @param paIndex Wether the index is an index made up by PulseAudio (pa_sink_info->index), or the index
     * in the array
     * @return The PortModel with the ports for this sink.
     */
    Q_INVOKABLE
    PortModel* getPortModel(int index, bool paIndex = true);

    static void paSinkInfoCallback(pa_context* c, const pa_sink_info* info, int eol, void* userdata);
signals:
    void error(QString error);
private:
    QList<Sink> sinks;
    QList<PortModel> portModels;
    pa_context* context;
    bool doneRefreshing = false;

    QMutex refreshMutex;
    QWaitCondition refreshing;
};

#endif // SINKMODEL_H
