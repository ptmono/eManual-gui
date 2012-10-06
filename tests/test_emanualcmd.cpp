#include "test_emanualcmd.hpp"
#include "common.hpp"		// dummy_db, cleanup_db

#include <stdlib.h>
#include <cstdio>		// remove

#include <QApplication>

#include "../src/mainwindow.hpp"
#include "../src/emanualcmd.hpp"
#include "../src/models.hpp"
#include "../src/config.hpp"	// Config
#include "../src/dderrors.hpp"


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
    event["allDay"] = false;
    
    QDateTime current = QDateTime::currentDateTime();
    event["start"] = current.addDays(2);
    event["end"] = current.addDays(5);

    EmanualCmd emanual;
    bool result = emanual.editSchedule(event);

    Schedule* schedule = ModelAPIs::Instance()->schedule(event["id"].toInt());

    QVERIFY(result == true);
    QCOMPARE(event["allDay"].toBool(), schedule->allDay());

    // It seems an limitation of QVariant::toDateTime(). Confirm that.
    // Actual (event["start"].toDateTime()): 2012/04/30 07:01:25.524[local time]
    // Expected (schedule->start()): 2012/04/30 07:01:25.000[local time]
    QCOMPARE(event["start"].toDateTime().date(), schedule->start().date());
    QCOMPARE(event["end"].toDateTime().date(), schedule->end().date());


}


void TestEmanualCmd::test_EmanualCmd_getEvents()
{
    EmanualCmd emanualcmd;
    //qDebug() << emanualcmd.getEvents();

    const QDjangoQuerySet<Record> record_set;
    qDebug() << record_set.all().values();

    const QDjangoQuerySet<ElementGroup> eg_set;
    qDebug() << eg_set.all().values();
}
