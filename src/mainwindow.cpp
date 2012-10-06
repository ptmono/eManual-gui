#include "mainwindow.hpp"
#include "emanualcmd.hpp"
#include "dialogs.hpp"		// EventCreatorDlg, ElementCreatorDlg, GroupCreatorDlg

#include <QWebFrame>
#include <QMessageBox>

EmanualCmd* MainWin::m_ecmd = NULL;


MainWin::MainWin(QWidget *parent) : QWebView(parent)
{
    m_ecmd = new EmanualCmd(this);

    QObject::connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
		     this, SLOT(addJSObject()));
    
    QUrl startURL = QUrl("tools/fullcalendar-1.5.3/demos/selectable.html");
    setUrl(startURL);

    createActions();
}

QSize MainWin::sizeHint() const
{
    return QSize(1100, 800);
}

void MainWin::addJSObject()
{
    page()->mainFrame()->addToJavaScriptWindowObject(QString("emanualCmd"), m_ecmd);
}

void MainWin::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    // menu.addAction(createEventAction);
    menu.addAction(createElementAction);
    menu.addAction(createGroupAction);
    menu.addAction(recordAction);

    menu.exec(event->globalPos());
}


void MainWin::createActions()
{
    // createEventAction = new QAction(tr("Create &Event"), this);
    // connect(createEventAction, SIGNAL(triggered()), this, SLOT(createEvent()));

    createElementAction = new QAction(tr("Create E&lement"), this);
    connect(createElementAction, SIGNAL(triggered()), this, SLOT(createElement()));

    createGroupAction = new QAction(tr("Create &Group"), this);
    connect(createGroupAction, SIGNAL(triggered()), this, SLOT(createGroup()));

    recordAction = new QAction(tr("Add &Record"), this);
    connect(recordAction, SIGNAL(triggered()), this, SLOT(addRecord()));
}


void MainWin::createEvent()
{
    // EventCreatorDlg event_dlg(this);
    // event_dlg.exec();
}

void MainWin::createElement()
{
    ElementCreatorDlg element_dlg(this);
    if (element_dlg.exec() == QDialog::Accepted){
	bool result = element_dlg.save();
	if (!result){
	    QMessageBox msgBox(this);
	    msgBox.setText("Something is wrong!");
	    msgBox.exec();
	}
    }
}

void MainWin::createGroup()
{
    GroupCreatorDlg group_dlg(this);
    if (group_dlg.exec() == QDialog::Accepted){
	bool result = group_dlg.save();
	if (!result){
	    QMessageBox msgBox(this);
	    msgBox.setText("We couldn't create new group");
	    msgBox.exec();
	}
    }
}
	    
void MainWin::addRecord()
{
    InputDlg input_dlg(this);
    if (input_dlg.exec() == QDialog::Accepted){
	input_dlg.save();
    }
}

