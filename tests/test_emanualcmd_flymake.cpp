#include "test_emanualcmd.hpp"
#include "common.hpp"		// dummy_db, cleanup_db

#include <stdlib.h>
#include <cstdio>		// remove

#include <QApplication>

#include "../src/mainwindow.hpp"
#include "../src/emanualcmd.hpp"
#include "../src/models.hpp"
#include "../src/config.hpp"	// Config

void TestEmanualCmd::initTestCase()
{
    
    remove(Config::Instance()->getTestDbName().c_str());
    initdb();
    dummy_db();
}

// TODO: TestModels error. Why?
void TestEmanualCmd::test_gui()
{
    int test_gui = 0;
    
    if (test_gui){
	int argc = 0;
	char* argv[1] = {0};
	QApplication app(argc, argv);
	
	MainWin win;
	win.show();
	app.exec();
    }
}

void TestEmanualCmd::cleanupTestCase()
{
    cleanup_db();
}


void TestEmanualCmd::test_EmanualCmd_editSchedule()
{
    QVariantMap event;
    event["id"] = 1;		// id is for schedule
    event["name"] = "running";
    event["allDay"] = false;
    
    QDateTime current = QDateTime::currentDateTime();
    event["start"] = current;
    event["end"] = QDateTime(QDate(current.date().year(), current.date().month(), current.date().day() + 3), current.time());

    qDebug() << current.time();

    int year = current.date().year();
    int month = current.date().month();
    int day = current.date().day() + 3); 

QDate date = QDate(year, month, day);
qDebug() << date;
    qDebug() << event;

    qDebug() << current.date().month();
    qDebug() << current.date().day() + 3;
    
}
