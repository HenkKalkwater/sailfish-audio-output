#ifndef SINK_H
#define SINK_H

#include <QObject>
#include <QDebug>
#include <QString>

#include <pulse/pulseaudio.h>

#include "portmodel.h"

/**
 * @brief Represents a PulseAudio sink
 */
class Sink : public QObject
{
    Q_OBJECT
public:
    explicit Sink(const Sink& other);
    explicit Sink(const pa_sink_info* sink, QObject *parent = nullptr);
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(int index READ index CONSTANT)

    float volume() const { return m_volume; }
    void setVolume(float newVolume);

    QString name() const { return m_name; }
    QString description() const { return m_description; }
    int index() const { return m_index; }


signals:
    void volumeChanged();
public slots:
private:
    const pa_sink_info* sink;
    QString m_name;
    QString m_description;
    float m_volume;
    int m_index;

};

#endif // SINK_H
