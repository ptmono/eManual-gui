#ifndef EMANUALCMD_HPP
#define EMANUALCMD_HPP

#include <QtGui>

class EmanualCmd : public QObject
{
    Q_OBJECT

public:
    EmanualCmd(QObject *parent = 0);
    ~EmanualCmd();

public slots:
    bool editSchedule(QVariantMap event);
    QVariantMap startScheduleDlg(QString start, QString end, bool allDay);

    QVariantList getEvents() const;
    QList<QString> getUnits() const;
    int getMtest() const{ return m_test; };

private:
    int m_test;
    QWidget* m_parent;
    
signals:
    void finishedUpdate(QVariantList events);
    void endedScheduleDlg(QVariantList events);
};

#endif

