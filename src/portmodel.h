#ifndef PORTMODEL_H
#define PORTMODEL_H

#include <QAbstractListModel>
#include <QProcess>
#include <QObject>
#include <QDebug>

struct Port {
	QString sink;
	QString sinkDescription;
	QString shortName;
	QString longName;
	int priority;
	bool available;
	bool active;
	Port() {}
	/*Port(QString port, QString shortName, QString longName, int priority, bool available, bool active) {
		this->port = port;
		this->shortName = shortName;
		this->longName = longName;
		this->priority = priority;
		this->available = available;
		this->active = active;
	}*/
};

class PortModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum PortRole {
		SHORT_NAME = Qt::UserRole + 1,
		LONG_NAME,
		PRIORITY,
		AVAILABLE,
		ACTIVE,
		SINK,
		SINK_DESCRIPTION
	};
	Q_PROPERTY(int roleShortName READ roleShortName)
	Q_PROPERTY(int activeIndex READ activeIndex NOTIFY activeIndexChanged)
	explicit PortModel(QObject *parent = nullptr);

	Q_INVOKABLE
	void update(bool force = false);
	Q_INVOKABLE
	QString nameOf(int index) {
		if (index >= 0 && index < ports.size()) return ports[index].shortName;
		return "";
	}
	Q_INVOKABLE
	QString sinkOf(int index) {
		if (index >= 0 && index < ports.size()) return ports[index].sink;
		return "";
	}
	int activeIndex() { return m_activeIndex; }

	int rowCount(const QModelIndex &parent) const override;
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	int roleShortName() { return ROLE_SHORT_NAME; }
signals:
	void activeIndexChanged(int newIndex);
	void error(QString message);
private:
	QList<Port> ports;
	bool firstRun = true;
	int m_activeIndex = 0;
	int ROLE_SHORT_NAME = SHORT_NAME;
};

#endif // PORTMODEL_H
