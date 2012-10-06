#include "test_models.hpp"
#include "common.hpp"		// dummy_db, cleanup_db

#include <stdlib.h>
#include <cstdio>		// remove

#include <QDebug>
#include <QSqlDatabase>		// QSqlDatabase
#include <QSqlDriver>

#include "../src/models.hpp"	// Unit, Element, Group, ElementGroup
#include "../src/config.hpp"
#include "../src/emanualcmd.hpp"
#include "../src/dderrors.hpp"

extern void initdb();

void TestModels::initTestCase()
{
    remove(Config::Instance()->getTestDbName().c_str());    
    initdb();
    dummy_db();			//Create dummy database.
}

void TestModels::cleanupTestCase()
{
    cleanup_db();
}


void TestModels::test_common()
{
    const QDjangoQuerySet<Unit> units;
    QList< QMap<QString, QVariant> > unit_map = units.all().values();
    //QCOMPARE(unit_map.size(), 4); //See initdb()
    
    const QDjangoQuerySet<Element> elements;
    QList< QMap<QString, QVariant> > element_map = elements.all().values();
    QCOMPARE(element_map.size(), 3);

    const QDjangoQuerySet<Group> groups;
    QList< QMap<QString, QVariant> > group_map = groups.all().values();
    QCOMPARE(group_map.size(), 2);

    const QDjangoQuerySet<ElementGroup> elementgroups;
    QList< QMap<QString, QVariant> > elementgroup_map = elementgroups.all().values();
    QCOMPARE(elementgroup_map.size(), 1);

    const QDjangoQuerySet<Schedule> schedules;
    QList< QMap<QString, QVariant> > schedule_map = schedules.all().values();
    QCOMPARE(schedule_map.size(), 7);

    const QDjangoQuerySet<Record> records;
    QList< QMap<QString, QVariant> > record_map = records.all(). values();
    QCOMPARE(record_map.size(), 1);
}

/// Schedule
void TestModels::test_Schedule()
{
    QDateTime current = QDateTime().currentDateTime();
    Schedule schedule;
    schedule.setStart(current);
    QVERIFY(schedule.end() == current);
}

/// Record
void TestModels::test_Record()
{
    QDateTime rdate = QDateTime().currentDateTime();
    QString input = "pushup 300";

    QStringList result = input.split(" ");
    QString name = result[0];
    QString record_str = result[1];
    //int record = record_str.toUInt(); // Returns 0 when failed
    
    QVERIFY(ModelAPIs::element(name) != 0);
    QVERIFY(ModelAPIs::element("pushus") == 0);

    // Correct input
    Record record;
    record.fromInput(input);
    QVERIFY(record.save() == true);

    // Input with shortcut
    QString input_shortcut = "push 500";
    Record record_shortcut;
    record_shortcut.fromInput(input_shortcut);
    QVERIFY(record_shortcut.save() == true);

    // Bad record
    QString input_bad = "pushup 300a";

    Record record_to_be_failed;
    try{
	record_to_be_failed.fromInput(input_bad);
	QVERIFY(false);
    }
    catch (exception& e){
	QVERIFY(true);
    }

    // There is no element
    QString input_bad2 = "no_element 300";
    Record record_to_be_failed2;
    try{
	record_to_be_failed2.fromInput(input_bad2);
	QVERIFY(false);
    }
    catch (exception& e){
	QVERIFY(true);
    }
    
}

void TestModels::test_RecordSchedule()
{
    // Continue test_Record
    const QDjangoQuerySet<Record> record_set;
    //qDebug() << record_set.all().values();

    QString input = "pushup 500";
    Record* record = new Record();
    record->fromInput(input);
    record->save();
    
    // Save
    // We need the list of today's schedule
    
    
}



/// ModelAPIs
void TestModels::test_ModelAPIs_schedules()
{
    const QDjangoQuerySet<Schedule> schedules_set;
    QList< QMap<QString, QVariant> > schedule_map = schedules_set.all().values();
		
    QVariantList schedules;
    schedules = ModelAPIs::Instance()->schedulesToQVariantList();
    //qDebug() << schedules;

    QVERIFY(schedule_map.count() == schedules.count());
			  
    // If element_id is null then group_id is the title.
    QVERIFY(schedules[0].toMap()["title"].toString() == "pushup"); // element title
    QVERIFY(schedules[1].toMap()["title"].toString() == "daily");  // group title
    //qDebug() << schedule_map;
    
}

void TestModels::test_ModelAPIs_units()
{
    // Our result
    const QDjangoQuerySet<Unit> units_set;
    QList< QList<QVariant> > units = units_set.all().valuesList();

    QList<QString> result;
    for (int i = 0; i < units.count(); i++){
	result << units[i][1].toString();
    }

    // From method
    QList<QString> result2;
    result2 = ModelAPIs::Instance()->units();
    QCOMPARE(result, result2);
}

void TestModels::test_ModelAPIs_elements()
{
    const QDjangoQuerySet<Element> elements_set;
    QList< QMap<QString, QVariant> > elements_map = elements_set.all().values();

    //qDebug() << elements_map;
    QList<QString> result;
    for (int i = 0; i < elements_map.count(); i++){
     	result << elements_map[i]["name"].toString();
    }

    QList<QString> result2;
    result2 = ModelAPIs::Instance()->elements();
    QCOMPARE(result, result2);
}

void TestModels::test_ModelAPIs_groups()
{
    const QDjangoQuerySet<Group> groups_set;
    QList< QMap<QString, QVariant> > groups_map = groups_set.all().values();

    QList<QString> result;
    for (int i = 0; i < groups_map.count(); i++){
	result << groups_map[i]["name"].toString();
    }

    QList<QString> result2;
    result2 = ModelAPIs::Instance()->groups();
    QCOMPARE(result, result2);
}

void TestModels::test_ModelAPIs_unit()
{
    // // get an existing user
    // other = users.get(QDjangoWhere("username", QDjangoWhere::Equals, "foouser"));
    // QVERIFY(other != 0);
    // QCOMPARE(other->username(), QLatin1String("foouser"));
    // QCOMPARE(other->password(), QLatin1String("foopass"));

    const QDjangoQuerySet<Unit> units_set;
    Unit* unit = units_set.get(QDjangoWhere("name", QDjangoWhere::Equals, "second"));

    //qDebug() << unit->name();
    Unit* unit2;
    unit2 = ModelAPIs::unit("second");

    QCOMPARE(unit->name(), unit2->name());

    // There is no element.
    Unit* unit3;
    unit3 = ModelAPIs::unit("no_exist");
    QVERIFY(unit3 == 0);
}

void TestModels::test_ModelAPIs_schedule()
{
    const QDjangoQuerySet<Schedule> schedule_set;
    Schedule* schedule = schedule_set.get(QDjangoWhere("id", QDjangoWhere::Equals, 2));

    Schedule* schedule2 = ModelAPIs::Instance()->schedule(2);
    QCOMPARE(schedule->start(), schedule2->start());
}

void TestModels::test_ModelAPIs_lastSchedule()
{
    
    const QDjangoQuerySet<Schedule> schedule_set;
    QList< QMap<QString, QVariant> > schedule_map = schedule_set.all().values();

    
    QCOMPARE(schedule_map.last(), ModelAPIs::Instance()->lastSchedule());
}

void TestModels::test_ModelAPIs_elementFromShortcut()
{
    QVERIFY(ModelAPIs::elementFromShortcut("push") != 0);
}



// TODO: complete this to work. Or determine how to do this.
void TestModels::test_ModelAPIs_currentSchedule()
{
    Schedule* sch = ModelAPIs::currentSchedule("running");
    QVERIFY(sch->element()->name() == "running");
    QVERIFY(ModelAPIs::currentSchedule("noName") == 0);
}

