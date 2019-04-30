#include "portmodel.h"

PortModel::PortModel(pa_context* context, QObject *parent)
	: QAbstractListModel(parent) {
    this->context = context;
}

QVariant PortModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	switch (role) {
	case SHORT_NAME:
		return ports[index.row()].shortName;
	case LONG_NAME:
		return ports[index.row()].longName;
	case AVAILABLE:
		return ports[index.row()].available;
	case PRIORITY:
		return ports[index.row()].priority;
	case ACTIVE:
		return ports[index.row()].active;
	case SINK:
		return ports[index.row()].sink;
	case SINK_DESCRIPTION:
		return ports[index.row()].sinkDescription;
	default:
		return QVariant();
	}
}

int PortModel::rowCount(const QModelIndex &parent) const {
	return ports.size();
}

void PortModel::update(bool force) {
	bool shouldReset = this->firstRun || force;
	this->firstRun = false;
	// Start command
	QProcess listCommand;
	QProcessEnvironment env;

	// The command might output different things in other languages
	// but our parsing method is based on English.
	env.insert(listCommand.processEnvironment().systemEnvironment());
	env.insert("LC_ALL", "C");
	listCommand.setProcessEnvironment(env);
	listCommand.start("pactl", QStringList() << "list" << "sinks");
	// Wait until it is finished.
	listCommand.waitForFinished(-1);
	QString output = listCommand.readAllStandardOutput();
	QString errorOut = listCommand.readAllStandardError();
	if (!errorOut.isEmpty()) {
		qDebug() << "Error: " << errorOut;
		qDebug() << output;
		emit error(errorOut);
	}
	//qDebug() << "OUTPUT: " << output;
	QVector<QStringRef> lines = output.splitRef("\n");

	if (shouldReset) {
		this->beginResetModel();
		ports.clear();
	}

	int indentation = 0;
	QStringRef currentSink;
	QStringRef currentSinkDescription;
	// Workaround so that the cover only goes to 1 of the active sinks
	bool firstActive = false;
	bool inPortSection = false;
	for (QStringRef line : lines) {
		for (indentation = 0; indentation < line.length(); indentation++) {
			if (line.data()[indentation] != '\t') break;
		}
		if (line.startsWith("\tName: ")) currentSink = line.mid(7);
		if (line.startsWith("\tDescription: ")) currentSinkDescription = line.mid(14);
		if (inPortSection && indentation < 2) inPortSection = false;
		if (line.startsWith("\tActive Port:")) {
			QStringRef activePortName = line.mid(14);
			//qDebug() << activePortName;
			int i = 0;
			for (QList<Port>::iterator port = ports.begin(); port != ports.end(); port++) {
				if (port->sink == currentSink && port->shortName == activePortName) {
					if (!port->active) {
						if (firstActive) {
							this->m_activeIndex = i;
							emit activeIndexChanged(i);
							firstActive = false;
						}
						qDebug() << "Active: " + port->shortName << " (i: " << i << ")";
						port->active = true;
						this->dataChanged(this->index(i), this->index(i));
					}
				} else if(port->sink == currentSink && port->active == true) {
					port->active = false;
					this->dataChanged(this->index(i), this->index(i));
				}
				i++;
			}
		}
		if (line.startsWith("\tPorts:")) {
			inPortSection = true;
			continue;
		}
		//qDebug() << line;
		//qDebug() << "CurrentPort: " << currentPort;

		if (inPortSection && shouldReset) {
			// FORMAT: \t\tsystem-name: Human-readable name (priority: 777, available/not available)
			int firstColon = line.indexOf(':');
			int firstBracket = line.lastIndexOf('(');
			int firstColonFromBracket = line.indexOf(':', firstBracket);
			int firstCommaFromBracket = line.indexOf(',', firstBracket);
			Port port;
			port.sink = currentSink.toString();
			port.sinkDescription = currentSinkDescription.toString();
			port.shortName = line.mid(2, firstColon - 2).toString();
			port.longName = line.mid(firstColon + 2, firstBracket - firstColon - 2).toString();
			port.priority = line.mid(firstColonFromBracket + 2, firstCommaFromBracket - firstColonFromBracket - 2).toInt();
			port.available = line.mid(firstCommaFromBracket + 2).startsWith("available");
			this->ports.append(port);
			//qDebug() << "Found " << port.shortName;
		}

	}
	if (shouldReset) {
		this->endResetModel();
	}
}

QHash<int, QByteArray> PortModel::roleNames() const {
	QHash<int, QByteArray> result;
	result[SINK] = "sink";
	result[SINK_DESCRIPTION] = "sinkDescription";
	result[SHORT_NAME] = "shortName";
	result[LONG_NAME] = "longName";
	result[AVAILABLE] = "available";
	result[PRIORITY] = "priority";
	result[ACTIVE] = "active";
	return result;
}
