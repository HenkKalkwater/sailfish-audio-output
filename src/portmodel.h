#ifndef PORTMODEL_H
#define PORTMODEL_H

#include <QAbstractListModel>
#include <QProcess>
#include <QObject>
#include <QDebug>

#include <pulse/pulseaudio.h>

#include "port.h"

class PortModel : public QAbstractListModel
{
	Q_OBJECT
public:
    PortModel() : PortModel(0, nullptr, 0, 0){}
    explicit PortModel(const PortModel& other);
    explicit PortModel(int index, pa_sink_port_info** info, size_t info_length, pa_sink_port_info* activeIndex,
                       QObject *parent = nullptr);
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
     * @return the index of the sink these ports belong to
     */
    int index() const { return m_index; }

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void activeIndexChanged(int newIndex);
    void error(QString message);
private:
    QList<Port> ports;
    // Can't get it to work with a QList
    //pa_sink_port_info** ports;
    //size_t port_count;

    pa_sink_port_info* m_activePort;
    const int m_index;
    bool active;
};
Q_DECLARE_METATYPE(PortModel)
#endif // PORTMODEL_H
