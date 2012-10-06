#include <QTest>
#include <QtGui>
#include <QDebug>

#include "../src/models.hpp"
#include "../src/config.hpp"	// Config

/** 
 * Dummy database for testing. If you want to edit a data, you also edit
 * test_models.cpp TestModels::test_common.
 *
 */
void dummy_db()
{
    // initially created in initdb()
    // Unit count;
    // count.setName("count");
    // QCOMPARE(count.save(), true);
    
    Unit second;
    second.setName("second");
    QCOMPARE(second.save(), true);

    const QDjangoQuerySet<Unit> units;
    Unit* f_second = units.get(QDjangoWhere("name", QDjangoWhere::Equals, "second"));
    
    Element pushup;
    pushup.setName("pushup");
    pushup.setPurpose(1000);
    pushup.setNote("It is pushup note.");
    pushup.setShortcut("push");
    pushup.setUnit(f_second);
    QCOMPARE(pushup.save(), true);

    Element* running = new Element;
    running->setName("running");
    running->setPurpose(300);
    running->setNote("It is running note.");
    QCOMPARE(running->save(), true);

    Element breathing;
    breathing.setName("breathing");
    breathing.setPurpose(70000);
    QCOMPARE(breathing.save(), true);

    Group daily_group;
    daily_group.setName("daily");
    daily_group.setNote("It is daily.");
    QCOMPARE(daily_group.save(), true);

    Group monthly_group;
    monthly_group.setName("monthly");
    monthly_group.setNote("It is monthly");
    QCOMPARE(monthly_group.save(), true);

    const QDjangoQuerySet<Element> elements;
    Element* f_pushup = elements.get(QDjangoWhere("name", QDjangoWhere::Equals, "pushup"));
    Element* f_running = elements.get(QDjangoWhere("name", QDjangoWhere::Equals, "running"));
    
    const QDjangoQuerySet<Group> groups;
    Group* f_daily_group = groups.get(QDjangoWhere("name", QDjangoWhere::Equals, "daily"));


    // It requires free store. It requires more code.
    ElementGroup daily;
    daily.setElement(f_pushup);
    daily.setGroup(f_daily_group);
    QCOMPARE(daily.save(), true);


    QDateTime current = QDateTime::currentDateTime();
    int year = current.date().year();
    int month = current.date().month();
    int day = current.date().day();

    QDate ddate = QDate(year, month, current.date().addDays(1).day());
    QTime dtime = QTime(10, 5, 59);
    QDateTime start = QDateTime(ddate, dtime);
    QDateTime end = QDateTime(QDate(year, month, current.date().addDays(3).day()), QTime(15, 6, 5));

    Schedule today;		// start: today + 1, end: today + 3
    today.setStart(start);
    today.setEnd(end);
    today.setElement(f_pushup);
    QCOMPARE(today.save(), true);

    QDate ddate2 = QDate(year, month, day);
    QDateTime start2 = QDateTime(ddate2);
    Schedule today2;		// start: today0
    today2.setStart(start2);
    today2.setEnd(start2);
    today2.setGroup(f_daily_group);
    QCOMPARE(today2.save(), true);

    Schedule today3;		// start: today0, end: today0 + 1
    today3.setStart(start2);
    today3.setEnd(start2.addDays(1));
    today3.setElement(f_pushup);
    QCOMPARE(today3.save(), true);

    Schedule today4;		// start: today, end: today + 3
    today4.setStart(current);
    today4.setEnd(current.addDays(3));
    today4.setElement(f_pushup);
    QCOMPARE(today4.save(), true);

    Schedule today5;		// start: today0 + 1, end: today0 + 2
    today5.setStart(start2.addDays(1));
    today5.setEnd(current.addDays(2));
    today5.setElement(f_pushup);
    QCOMPARE(today5.save(), true);

    Schedule yesterday;		// start: today0 - 1, end: today0 + 3
    yesterday.setStart(start2.addDays(-1));
    yesterday.setEnd(start2.addDays(3));
    yesterday.setElement(f_pushup);
    QCOMPARE(yesterday.save(), true);

    Schedule yesterday2;
    yesterday2.setStart(start2.addDays(-1));
    yesterday2.setEnd(start2.addDays(4));
    yesterday2.setElement(running);
    QCOMPARE(yesterday2.save(), true);

    QDateTime rdate = QDateTime(QDate(year, month, day), QTime(18, 0, 0));
    Record record_of_today;
    record_of_today.setDatetime(rdate);
    record_of_today.setRecord(800);
    record_of_today.setElement(f_pushup);
    QCOMPARE(record_of_today.save(), true);
}

void cleanup_db()
{
    // There is cleanning method.
    //QCOMPARE(QDjangoQuerySet<E>().remove(), true);

    // Will drop all the registerModels.
    QCOMPARE(QDjango::registerModel<Unit>().dropTable(), true);
    QCOMPARE(QDjango::registerModel<Element>().dropTable(), true);
    QCOMPARE(QDjango::registerModel<Group>().dropTable(), true);
    QCOMPARE(QDjango::registerModel<ElementGroup>().dropTable(), true);
    QCOMPARE(QDjango::registerModel<Schedule>().dropTable(), true);
    QCOMPARE(QDjango::registerModel<Record>().dropTable(), true);
    remove(Config::Instance()->getTestDbName().c_str());
}
