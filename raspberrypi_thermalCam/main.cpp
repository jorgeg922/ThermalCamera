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
	//thread->enable(); //YOU MUST ENABLE BEFORE START THREAD 
	thread->start();  
	return a.exec();
}

