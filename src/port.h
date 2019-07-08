#ifndef PORT_H
#define PORT_H

#include <QObject>

#include <pulse/pulseaudio.h>

/**
 * @brief Represents a PulseAudio sink port
 */
class Port : public QObject {
    Q_OBJECT
public:
    Port(const pa_sink_port_info* port, size_t index, QObject* parent = nullptr);
    Port(const Port& other);
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(int priority READ priority)
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(size_t index READ index)

    QString name() const { return m_name; }
    QString description() const { return m_description; }
    int priority() const { return m_priority; }
    bool available() const { return m_available; }
    size_t index() const { return m_index; }

    //Port &operator=(const Port &other);
    void setAvailable(bool isAvailable);

    const pa_sink_port_info* rawInfo() const { return this->m_rawInfo; }
signals:
    void availableChanged();
private:
    const pa_sink_port_info* m_rawInfo;
    QString m_name;
    QString m_description;
    int m_priority;
    bool m_available;
    size_t m_index;
};

#endif // PORT_H
