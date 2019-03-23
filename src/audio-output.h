#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QString>
#include <QDebug>
#include "portmodel.h"

class Listener: public QObject
{
	Q_OBJECT
public:
	explicit Listener(QObject* parent = 0) {}
	~Listener() {}
public slots:
	void changeOutput(const QString &sink, const QString &port) {
		qDebug() << "Changing output to sink: " << sink << ", port: " << port;
		system(qPrintable("pactl set-sink-port " + sink + " " + port));
	}
};

#endif // AUDIOOUTPUT_H
