#ifndef PORTMODEL_H
#define PORTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QDebug>
#include <QList>

#include <algorithm>
#include <iterator>

#include <pulse/pulseaudio.h>

#include "port.h"

/**
 * @brief Contains a list with all the ports for a single sink.
 *
 * This class contains a list with the ports for a single list. Ports are stored
 * as a Port. This class is referenced by SinkModel, which also passes relevant
 * events from the PulseAudio API to here.
 */
class PortModel : public QAbstractListModel
{
	Q_OBJECT
public:
    explicit PortModel();
    /**
     * @brief Copy constructor
     * @param other The other model to copy from.
     */
    explicit PortModel(const PortModel& other);

    /**
     * @brief Create a PortModel of a pa_sink_info object (see Pulseaudio documentation).
     * @param info The PulseAudio object to generate this model of.
     * @param parent The parent QObject.
     */
    explicit PortModel(pa_context* context, const pa_sink_info* info, QObject *parent = nullptr);

    /**
     * @brief Roles of properties exposed to QML.
     */
    enum PortRole {
        NAME = Qt::UserRole + 1,
        DESCRIPTION,
        INDEX,
        AVAILABLE,
        PRIORITY,
        ACTIVE
    };

    Q_PROPERTY(int index READ index CONSTANT)

    /**
     * @return the PulseAudio defined index of the sink these ports belong to.
     */
    int index() const { return m_index; }

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Update information of this port by calling the PulseAudio API.
     */
    void update();

    Q_INVOKABLE
    void setActivePort(QString name);

    static void paSinkInfoCallback(pa_context* c, const pa_sink_info* info, int eol, void* userdata);
signals:
    void activePortChanged(int newIndex);
    void error(QString message);
private:
    QList<Port> ports;
    pa_context* m_context;
    const pa_sink_info* m_sinkInfo;
    size_t m_activeIndex;
    const int m_index;
    bool active;

    /**
     * @brief findActivePortIndex Returns the index of the given port in `ports`
     * @param port The port to look for
     * @return The index if found, or -1
     */
    int findPortIndex(pa_sink_port_info* port);
};
#endif // PORTMODEL_H
