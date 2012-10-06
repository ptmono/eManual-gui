#ifndef MODELS_HPP
#define MODELS_HPP

#include <QDjango.h>
#include <QDjangoModel.h>
#include <QDjangoQuerySet.h>
#include <QDjangoWhere.h>

#include <QDateTime>

class Unit : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    //Q_CLASSINFO("name", "max_lenth=12 primary_key=true db_index=true")
    Q_CLASSINFO("name", "max_lenth=12")

public:
    Unit(QObject* parent = 0);

    QString name() const;
    void setName(const QString &name);

private:
    QString m_name;
};


class Element : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int purpose READ purpose WRITE setPurpose)
    Q_PROPERTY(QString note READ note WRITE setNote)
    Q_PROPERTY(QString shortcut READ shortcut WRITE setShortcut)
    Q_PROPERTY(Unit* unit READ unit WRITE setUnit)

    Q_CLASSINFO("name", "max_lenth=30 primary_key=true db_index=true")
    Q_CLASSINFO("note", "max_lenth=300 null=true")
    Q_CLASSINFO("shortcut", "max_lenth=30 null=true")
    Q_CLASSINFO("unit", "null=true")

public:
    Element(QObject* parent = 0);

    QString name() const;
    void setName(const QString &name);

    int purpose() const;
    void setPurpose(int purpose);

    QString note() const;
    void setNote(const QString &note);

    QString shortcut() const;
    void setShortcut(const QString &shortcut);

    Unit* unit() const;
    void setUnit(Unit *unit);
    
private:
    QString m_name;
    int m_purpose;
    QString m_note;
    QString m_shortcut;
};


class Group : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString note READ note WRITE setNote)

    Q_CLASSINFO("name", "max_lenth=30 primary_key=true db_index=ture")
    Q_CLASSINFO("note", "max_lenth=300 null=true")

public:
    Group(QObject *parent = 0);
    QString name() const;
    void setName(const QString &name);

    QString note() const;
    void setNote(const QString &note);

private:
    QString m_name;
    QString m_note;
};


class ElementGroup : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(Element* element READ element WRITE setElement)
    Q_PROPERTY(Group* group READ group WRITE setGroup)

    Q_CLASSINFO("element", "null=true")
    Q_CLASSINFO("group", "null=true")

public:
    ElementGroup(QObject *parent = 0);

    Element* element() const;
    void setElement(Element *element);

    Group* group() const;
    void setGroup(Group *group);

};


class Schedule : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(bool allDay READ allDay WRITE setAllDay)
    Q_PROPERTY(QDateTime start READ start WRITE setStart)
    Q_PROPERTY(QDateTime end READ end WRITE setEnd)

    Q_PROPERTY(Element* element READ element WRITE setElement)
    Q_PROPERTY(Group* group READ group WRITE setGroup)

    //Q_CLASSINFO("id", "primary_key=true auto_increment=true db_index=true unique=true")
    Q_CLASSINFO("end", "null=true")
    Q_CLASSINFO("element", "null=true")
    Q_CLASSINFO("group", "null=true")

public:
    Schedule(QObject *parent = 0);

    bool allDay() const;
    void setAllDay(const bool& allDay);

    QDateTime start() const;
    void setStart(const QDateTime& start);

    QDateTime end() const;
    void setEnd(const QDateTime& end);

    Element* element() const;
    void setElement(Element* element);

    Group* group() const;
    void setGroup(Group* group);

private:
    int m_id;
    bool m_allDay;
    QDateTime m_start;
    QDateTime m_end;
};


class Record : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(QDateTime datetime READ datetime WRITE setDatetime)
    Q_PROPERTY(int record READ record WRITE setRecord)
    Q_PROPERTY(Element* element READ element WRITE setElement)

    // All enable null. Because we can set record from fromInput method.
    Q_CLASSINFO("datetime", "null=true")
    Q_CLASSINFO("record", "null=true")
    Q_CLASSINFO("element", "null=true")

public:
    Record(QObject* parent = 0);
    void fromInput(const QString& input);

    QDateTime datetime() const;
    void setDatetime(const QDateTime& datetime);

    int record() const;
    void setRecord(int record);

    Element* element() const;
    void setElement(Element* element);

private:
    QStringList parseInput(const QString& input);
    QDateTime m_datetime;
    int m_record;
};


class RecordSchedule : public QDjangoModel
{
    Q_OBJECT
    Q_PROPERTY(Schedule* schedule READ schedule WRITE setSchedule)
    Q_PROPERTY(Record* record READ record WRITE setRecord)

public:
    RecordSchedule(QObject* parent = 0);
    Schedule* schedule() const;
    void setSchedule(Schedule* schedule);

    Record* record() const;
    void setRecord(Record* record);
};


class ModelAPIs
{
public:
    static ModelAPIs* Instance();
    /** 
     * Get all units.
     * @return string list of units.
     */
    static QList<QString> units();
    static QList<QString> schedules();
    static QVariantList schedulesToQVariantList();
    static QList<QString> elements();
    static QList<QString> groups();
    /** 
     * Get Unit object from database.
     *
     * @param name The name of unit.
     *
     * @return The Unit object. If not find, then returns 0.
     */
    static Unit* unit(const QString& name);
    static Element* element(const QString& name);
    static Element* elementFromShortcut(const QString& shortcut);
    static Group* group(const QString& name);
    static Schedule* schedule(int id);
    static QVariantMap lastSchedule();

    static QDjangoQuerySet<Schedule> todaySchedules();
    static Schedule* currentSchedule(const QString& name);

private:
    ModelAPIs(){};
    ModelAPIs(ModelAPIs const&){};
    ModelAPIs& operator=(ModelAPIs const& obj);
    
    static ModelAPIs* m_Instance;
};


void initdb();

#endif
