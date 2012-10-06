#include "models.hpp"
#include "config.hpp"
#include "libs.hpp"
#include "dderrors.hpp"

#include <boost/filesystem.hpp>
#include <cstdio>

// Unit
Unit::Unit(QObject *parent)
    : QDjangoModel(parent)
{
}

QString Unit::name() const
{
    return m_name;
}

void Unit::setName(const QString& name)
{
    m_name = name;
}


// Element
Element::Element(QObject* parent)
    : QDjangoModel(parent), m_purpose(0)
{
    const QDjangoQuerySet<Unit> units;
    Unit* f_second = units.get(QDjangoWhere("name", QDjangoWhere::Equals, "count"));
    setForeignKey("unit", f_second);
}

QString Element::name() const
{
    return m_name;
}

void Element::setName(const QString& name)
{
    m_name = name;
    
}

int Element::purpose() const
{
    return m_purpose;
}

void Element::setPurpose(int purpose)
{
    m_purpose = purpose;
}

QString Element::note() const
{
    return m_note;
}

void Element::setNote(const QString& note)
{
    m_note = note;
}

QString Element::shortcut() const
{
    return m_shortcut;
}

void Element::setShortcut(const QString& shortcut)
{
    m_shortcut = shortcut;
}

Unit* Element::unit() const
{
    return qobject_cast<Unit*>(foreignKey("unit"));
}

void Element::setUnit(Unit* unit)
{
    setForeignKey("unit", unit);
}


// Group
Group::Group(QObject* parent)
    : QDjangoModel(parent)
{
}

QString Group::name() const
{
    return m_name;
}

void Group::setName(const QString& name)
{
    m_name = name;
}

QString Group::note() const
{
    return m_note;
}

void Group::setNote(const QString& note)
{
    m_note = note;
}


// ElementGroup
ElementGroup::ElementGroup(QObject* parent)
    : QDjangoModel(parent)
{
    setForeignKey("element", new Element(this));
    setForeignKey("group", new Group(this));
}

Element* ElementGroup::element() const
{
    return qobject_cast<Element*>(foreignKey("element"));
}

void ElementGroup::setElement(Element* element)
{
    setForeignKey("element", element);
}

Group* ElementGroup::group() const
{
    return qobject_cast<Group*>(foreignKey("group"));
}

void ElementGroup::setGroup(Group* group)
{
    setForeignKey("group", group);
}


// Schedule
Schedule::Schedule(QObject* parent)
    : QDjangoModel(parent), m_allDay(true)
{
    setForeignKey("element", new Element(this));
    setForeignKey("group", new Group(this));
}

bool Schedule::allDay() const
{
    return m_allDay;
}

void Schedule::setAllDay(const bool& allDay)
{
    m_allDay = allDay;
}

QDateTime Schedule::start() const
{
    return m_start;
}

void Schedule::setStart(const QDateTime& start)
{
    m_start = start;
    if (m_end.isNull())
	m_end = start;
}

QDateTime Schedule::end() const
{
    return m_end;
}

void Schedule::setEnd(const QDateTime& end)
{
    m_end = end;
}

Element* Schedule::element() const
{
    return qobject_cast<Element*>(foreignKey("element"));
}

void Schedule::setElement(Element* element)
{
    setForeignKey("element", element);
}

Group* Schedule::group() const
{
    return qobject_cast<Group*>(foreignKey("group"));
}

void Schedule::setGroup(Group* group)
{
    setForeignKey("group", group);
}


// Record
Record::Record(QObject* parent)
    : QDjangoModel(parent), m_record(0)
{
    setForeignKey("element", new Element(this));
}

QDateTime Record::datetime() const
{
    return m_datetime;
}

void Record::fromInput(const QString& input)
{
    QStringList inputs = parseInput(input);

    // Get record
    m_record = inputs[1].toUInt(); // Returns 0 when failed
    // Handle for incorrect record
    if (m_record == 0)
	throw InputError();

    // Get element
    QString name = inputs[0];	// The name of element
    Element* element = ModelAPIs::element(name);
    if (element == 0)
	element = ModelAPIs::elementFromShortcut(name);
    if (element == 0)
	throw InputError();
    setElement(element);

    setDatetime(QDateTime().currentDateTime());
}

QStringList Record::parseInput(const QString& input)
{
    QStringList result = input.split(" ");
    if (result.size() < 2)
	throw InputError();
    return result;
}


void Record::setDatetime(const QDateTime& datetime)
{
    m_datetime = datetime;
}

int Record::record() const
{
    return m_record;
}

void Record::setRecord(int record)
{
    m_record = record;
}

Element* Record::element() const
{
    return qobject_cast<Element*>(foreignKey("element"));
}

void Record::setElement(Element* element)
{
    setForeignKey("element", element);
}


/// ScheduleRecord
RecordSchedule::RecordSchedule(QObject* parent)
    : QDjangoModel(parent)
{
    setForeignKey("schedule", new Schedule(this));
    setForeignKey("record", new Record(this));
}

Schedule* RecordSchedule::schedule() const
{
    return qobject_cast<Schedule*>(foreignKey("schedule"));
}

void RecordSchedule::setSchedule(Schedule* schedule)
{
    setForeignKey("schedule", schedule);
}

Record* RecordSchedule::record() const
{
    return qobject_cast<Record*>(foreignKey("record"));
}

void RecordSchedule:: setRecord(Record* record)
{
    setForeignKey("record", record);
}

Q_DECLARE_METATYPE(Element*)
Q_DECLARE_METATYPE(Group*)
Q_DECLARE_METATYPE(Unit*)
Q_DECLARE_METATYPE(ElementGroup*)
Q_DECLARE_METATYPE(Schedule*)


/// ModelAPIs
ModelAPIs* ModelAPIs::m_Instance = NULL;

ModelAPIs* ModelAPIs::Instance()
{
    if (!m_Instance){
	m_Instance = new ModelAPIs();
    }
    return m_Instance;
}

QList<QString> ModelAPIs::units()
{
    const QDjangoQuerySet<Unit> units_set;
    QList< QList<QVariant> > units = units_set.all().valuesList();
		 
    QList<QString> result;
    for (int i = 0; i < units.count(); i++){
	result << units[i][1].toString();
    }
    return result;
}

QList<QString> ModelAPIs::elements()
{
    const QDjangoQuerySet<Element> elements_set;
    QList< QMap<QString, QVariant> > elements_map = elements_set.all().values();

    QList<QString> result;
    for (int i = 0; i < elements_map.count(); i++){
     	result << elements_map[i]["name"].toString();
    }
    return result;
}

QList<QString> ModelAPIs::groups()
{
    const QDjangoQuerySet<Group> groups_set;
    QList< QMap<QString, QVariant> > groups_map = groups_set.all().values();

    QList<QString> result;
    for (int i = 0; i < groups_map.count(); i++){
	result << groups_map[i]["name"].toString();
    }
    return result;
}

QList<QString> ModelAPIs::schedules()
{
    const QDjangoQuerySet<Schedule> schedules_set;
    QList< QMap<QString, QVariant> > schedule_map = schedules_set.all().values();

    QList<QString> result;
    for (int i = 0; i < schedule_map.count(); i++){
	result << schedule_map[i]["start"].toString();
    }
    return result;
}

QVariantList ModelAPIs::schedulesToQVariantList()
{
    const QDjangoQuerySet<Schedule> schedule_set;
    QList< QMap<QString, QVariant> > schedule_map = schedule_set.all().values();

    QVariantList result;
    for (int i = 0; i < schedule_map.count(); i++){
	QMap<QString, QVariant> schedule = schedule_map[i];
	
	QVariantMap event;
	event.insert("id", schedule["id"]);
	if (schedule["element_id"] != "")
	    event.insert("title", schedule["element_id"]);
	else
	    event.insert("title", schedule["group_id"]);
	event.insert("start", schedule["start"]);
	if (schedule["end"] != ""){
	    event.insert("end", schedule["end"]);
	    // In fullcalendar allDayDefault is true. If allDay is true
	    // fullcalendar ignores time.
	    //event.insert("allDay", schedule["allDay"]);
	    event.insert("allDay",
			 stringToBool(schedule["allDay"].toString().toStdString().c_str()));
	}
	result << event;
    }
    return result;
    
}

static void __initdb_unit()
{
    Unit count;
    count.setName("count");
    count.save();

    Unit min;
    min.setName("min");
    min.save();

    Unit hour;
    hour.setName("hour");
    hour.save();
}

// TODO: Disable duplication connection warnning
void initdb()
{
    // initialize options
    QString databaseDriver = Config::Instance()->getDbDriver().c_str();
    QString databaseName = Config::Instance()->getDbName().c_str();

    bool exist_db_file = true;
    if ( !boost::filesystem::exists( databaseName.toStdString() ) )
    {
	exist_db_file = false;
    }


    //QString databaseName = "testtesttest.sqlite";
    QString databaseUser;
    QString databasePassword;
    QString databaseHost;
    
    // open database
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseDriver);
    db.setDatabaseName(databaseName);
    db.setUserName(databaseUser);
    db.setPassword(databasePassword);
    db.setHostName(databaseHost);
    if (!db.open()){
	fprintf(stderr, "Could not access database\n");
	return;
	}
    
    QDjango::setDatabase(db);

    // declare models
    QDjango::registerModel<Unit>();
    QDjango::registerModel<Element>();
    QDjango::registerModel<Group>();
    QDjango::registerModel<ElementGroup>();
    QDjango::registerModel<Schedule>();
    QDjango::registerModel<Record>();

    // Init database

    // We need to create tables.
    QDjango::createTables();
    if (!exist_db_file){
	__initdb_unit();
    }
    
}

Unit* ModelAPIs::unit(const QString& name)
{
    const QDjangoQuerySet<Unit> units_set;
    Unit* unit = units_set.get(QDjangoWhere("name", QDjangoWhere::Equals, name));
    return unit;
}

Element* ModelAPIs::element(const QString& name)
{
    const QDjangoQuerySet<Element> elements_set;
    Element* element = elements_set.get(QDjangoWhere("name", QDjangoWhere::Equals, name));
    return element;
}

Element* ModelAPIs::elementFromShortcut(const QString& shortcut)
{
    const QDjangoQuerySet<Element> elements_set;
    Element* element = elements_set.get(QDjangoWhere("shortcut", QDjangoWhere::Equals, shortcut));
    return element;
}

Group* ModelAPIs::group(const QString& name)
{
    const QDjangoQuerySet<Group> groups_set;
    Group* group = groups_set.get(QDjangoWhere("name", QDjangoWhere::Equals, name));
    return group;
}

Schedule* ModelAPIs::schedule(int id)
{
    const QDjangoQuerySet<Schedule> schedule_set;
    Schedule* schedule = schedule_set.get(QDjangoWhere("id", QDjangoWhere::Equals, id));
    return schedule;
}

QVariantMap ModelAPIs::lastSchedule()
{
    const QDjangoQuerySet<Schedule> schedule_set;
    QList< QMap<QString, QVariant> > schedule_map = schedule_set.all().values();

    QVariantMap result;
    result = schedule_map.last();
    // schedule["allDay"] is bool member. values() provides QString
    // instead of bool. It will used as bool. So convert this.
    result["allDay"] = result["allDay"].toBool();
    //qDebug() << "==:> ModelAPIs::lastSchedule()\n" << result;
    return result;
}

QDjangoQuerySet<Schedule> ModelAPIs::todaySchedules()
{
    // Get current schedules
    QDate date = QDate().currentDate();
    QDateTime today(date);
    const QDjangoQuerySet<Schedule> schedule_set;
    QDjangoQuerySet<Schedule> schedules;
    // today(0) is the start of today. e.g) "2012-06-06T00:00:00"
    // nextday(0) is the start of next day. e.g) "2012-06-07T00:00:00"
    // Out condition is
    // start < nextday(0) and end => today(0)
    schedules = schedule_set.filter(QDjangoWhere("start",
    						 QDjangoWhere::LessThan,
    						 today.addDays(1)));

    schedules = schedules.filter(QDjangoWhere("end",
    					      QDjangoWhere::GreaterOrEquals,
    					      today));

    // We can get the list that end is null with following. But Schedule
    // will set end as start if start is exist.
    // QDjangoQuerySet<Schedule> schedules_today_all;
    // schedules_today_all = schedules.exclude(QDjangoWhere("end",
    // 							 QDjangoWhere::IsNull,
    // 							 ""));
    return schedules;
}


Schedule* ModelAPIs::currentSchedule(const QString& name)
{
    QDjangoQuerySet<Schedule> schedules =
	ModelAPIs::todaySchedules();

    if (schedules.size() == 0)
	return 0;
    if (schedules.size() == 1)
	return schedules.at(0);

    // There are more two. Earlier start is first. And earlier end is
    // second. One rule, earlier is the schedule when superposition.
    QList<QVariantMap> schedules_map = schedules.values();

    QStringList order;
    order << "start";
    order << "end";
    QDjangoQuerySet<Schedule> schedules_ordered;
    schedules_ordered = schedules.orderBy(order);
    
    int count = 0;
    foreach(const Schedule& s, schedules_ordered){
	if (s.element() != 0 && s.element()->name() == name ){
	    return schedules_ordered.at(count);
	}
	count++;
    }
    
    return 0;
}
