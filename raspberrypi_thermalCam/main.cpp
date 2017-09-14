#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "MyLabel.h"

int main( int argc, char **argv )
{
	//create the app
	QApplication a( argc, argv );
	LeptonThread *thread = new LeptonThread(); //create object
	//thread->enable(); //must enable before starting the thread. Uncomment method in LeptonThread.cpp 
	thread->start();  
	return a.exec();
}

