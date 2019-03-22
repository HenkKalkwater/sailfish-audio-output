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
	void cppSlot(const QString &msg) {
		qDebug() << "Called the C++ slot with message:" << msg;
		system(qPrintable("pactl set-sink-port sink.primary " + msg));
	}
};

#endif // AUDIOOUTPUT_H
