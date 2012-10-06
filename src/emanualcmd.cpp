#include "emanualcmd.hpp"
#include "models.hpp"
#include "dialogs.hpp"
#include "dderrors.hpp"

#include <QDebug>
#include <QMessageBox>


EmanualCmd::EmanualCmd(QObject* parent)
    : QObject(parent)
{
    // Convert QObject* to QWidget*
    m_parent = dynamic_cast<QWidget*>(parent);
    // Let's initialize database to use.
    initdb();
}

EmanualCmd::~EmanualCmd()
{
}

QVariantList EmanualCmd::getEvents() const
{
    qDebug() << "==:> EmanualCmd::getEvents()" << ModelAPIs::Instance()->schedulesToQVariantList();

    return ModelAPIs::Instance()->schedulesToQVariantList();
}

QList<QString> EmanualCmd::getUnits() const
{
    return ModelAPIs::Instance()->units();
}

bool EmanualCmd::editSchedule(QVariantMap event)
{
    //qDebug() << "==:> EmanualCmd::editSchedule\n" << event;
    Schedule* schedule = ModelAPIs::Instance()->schedule(event["id"].toInt());
    schedule->setStart(event["start"].toDateTime());
    schedule->setEnd(event["end"].toDateTime());
    schedule->setAllDay(event["allDay"].toBool());
    return schedule->save();
}


QVariantMap EmanualCmd::startScheduleDlg(QString start, QString end, bool allDay)
{
    QVariantMap result;
    qDebug() << "==:> EmanualCmd::startScheduleDlg\n" << end;
    EventCreatorDlg eventCreatorDlg(start, end, allDay, m_parent);
    if(eventCreatorDlg.exec() == QDialog::Accepted){
	// This will check that event is saved.
	if (eventCreatorDlg.save()){
	    result = ModelAPIs::Instance()->lastSchedule();

	    if (result["element_id"] == "")
		result["title"] = result["group_id"]; // id. but has the name of element.
	    else
		result["title"] = result["element_id"];
	    qDebug() << "==:> EmanualCmd::startScheduleDlg\n" << result;
	    return result;
	}
    }
    return result;
}
