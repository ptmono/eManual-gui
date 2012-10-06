#ifndef DIALOGS_HPP
#define DIALOGSXS_HPP

#include <QtGui>

// Uic create Ui namespace.
#include "ui_createEventDlg.h"
#include "ui_createGroupDlg.h"
#include "ui_inputDlg.h"

class EventCreatorDlg : public QDialog, private Ui::CreateEventDlg
{
    Q_OBJECT

public:
    EventCreatorDlg(QDateTime start, QDateTime end, bool allDay, QWidget* parent = 0);
    EventCreatorDlg(QString start, QString end, bool allDay, QWidget* parent = 0);
    /** 
     * Save the event.
     * @return The name of element/group as string.
     */
    bool save();
    void init();
    QString title();

private slots:
    void setEmptyElementComboBox(int index);
    void setEmptyGroupComboBox(int index);
    
private:
    QDateTime m_start_dateTime;
    QDateTime m_end_dateTime;
    bool m_allDay;
};

class ElementCreatorDlg : public QDialog
{
    Q_OBJECT

public:
    ElementCreatorDlg(QWidget *parent = 0);
    ~ElementCreatorDlg();

    /** 
     * Save current element.
     * @return If success true, other false.
     */
    bool save();

private:
    QLabel* mp_elementLabel;
    QLabel* mp_purposeLabel;
    QLabel* mp_noteLabel;
    QLabel* mp_shortcutLabel;
    QLabel* mp_unitLabel;

    QLineEdit* mp_name;
    QSpinBox* mp_purpose;
    QTextEdit* mp_note;
    QLineEdit* mp_shortcut;
    QComboBox* mp_unit;

    QDialogButtonBox* mp_okButton;

    void initDlg();
};

class GroupCreatorDlg : public QDialog, private Ui::CreateGroupDlg
{
    Q_OBJECT

public:
    GroupCreatorDlg(QWidget* parent = 0);
    bool save();
    void init();

public slots:
    void addElementToGroupList();

private:
    QStringListModel* m_model;
};


class InputDlg : public QDialog, private Ui::InputDlg
{
    Q_OBJECT
    
public:
    InputDlg(QWidget* parent = 0);
    ~InputDlg();
    bool save();
    void init();
};

#endif
