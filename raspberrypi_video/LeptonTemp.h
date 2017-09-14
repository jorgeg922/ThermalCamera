#ifndef LEPTONTEMP_H
#define LEPTONTEMP_H

#include <ctime>
#include <stdint.h>

#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>

class LeptonTemp 
{
public:
	LeptonTemp();
	~LeptonTemp();
public slots:
	void doSomething();
};
#endif
