#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWebView>
#include "emanualcmd.hpp"

class MainWin : public QWebView
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = 0);

    // contextMenuEvent is virtual function to create right-click menu.
    // actions are required.
    void contextMenuEvent(QContextMenuEvent *event);
    QSize sizeHint() const;

			  			  
private slots:
    void addJSObject();

    // Slots of actions of right-click menu
    // Creating event is EmanualCmd::startScheduleDlg
    void createEvent();
    void createElement();
    void createGroup();

    void addRecord();
    
private:
    static EmanualCmd* m_ecmd;

    // actions of right-click menu
    //QAction* createEventAction;
    QAction* createElementAction;
    QAction* createGroupAction;
    QAction* recordAction;
    void createActions();
};

#endif
