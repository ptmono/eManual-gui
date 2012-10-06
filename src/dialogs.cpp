#include "dialogs.hpp"
#include "models.hpp"
#include "emanualcmd.hpp"
#include "dderrors.hpp"


/// EventCreatorDlg
EventCreatorDlg::EventCreatorDlg(QDateTime start, QDateTime end, bool allDay, QWidget* parent)
    : QDialog(parent), m_allDay(allDay)
{
    m_start_dateTime = start;
    m_start_dateTime = end;
    setupUi(this);
    init();
}

EventCreatorDlg::EventCreatorDlg(QString start, QString end, bool allDay, QWidget* parent)
    : QDialog(parent), m_allDay(allDay)
{
    QDateTime dateTime;
    m_start_dateTime = QDateTime::fromString(start, Qt::ISODate);
    //qDebug() << m_start_dateTime.toString(Qt::ISODate);
    m_end_dateTime = QDateTime::fromString(end, Qt::ISODate);
    setupUi(this);
    init();
}

void EventCreatorDlg::init()
{
    // Init element comboBox
    QList<QString> elements = ModelAPIs::Instance()->elements();
    m_elements->addItem("");
    for (int i = 0; i < elements.size(); ++i){
	m_elements->addItem(elements.at(i));
    }
    
    // Init group comboBox
    QList<QString> groups = ModelAPIs::Instance()->groups();
    m_groups->addItem("");
    for (int i = 0; i < groups.size(); ++i){
	m_groups->addItem(groups.at(i));
    }
    // Set start/end time
    m_start->setDateTime(m_start_dateTime);
    m_end->setDateTime(m_end_dateTime);

    connect(m_elements, SIGNAL(currentIndexChanged(int)), this, SLOT(setEmptyGroupComboBox(int)));
    connect(m_groups, SIGNAL(currentIndexChanged(int)), this, SLOT(setEmptyElementComboBox(int)));
}

bool EventCreatorDlg::save()
{
    Element* element = ModelAPIs::element(m_elements->currentText());
    Group* group = ModelAPIs::group(m_groups->currentText());

    if (!element and !group)
	// TODO: Create warnning window. "There is no element/group".
	return false;

    // Our start and end dateTime can be change.
    // Where m_start and m_end is QDateTimeEdit.
    m_start_dateTime = m_start->dateTime();
    m_end_dateTime = m_end->dateTime();

    // For debug
    // QList<QString> schedules = ModelAPIs::schedules();
    // qDebug() << schedules;

    Schedule schedule;
    schedule.setStart(m_start_dateTime);
    schedule.setEnd(m_end_dateTime);
    schedule.setAllDay(m_allDay);
    schedule.setElement(element);
    schedule.setGroup(group);
    bool result = schedule.save();

    return result;
}

void EventCreatorDlg::setEmptyElementComboBox(int index)
{
    m_elements->setCurrentIndex(0);
    m_groups->setCurrentIndex(index);
}

void EventCreatorDlg::setEmptyGroupComboBox(int index)
{
    m_groups->setCurrentIndex(0);
    m_elements->setCurrentIndex(index);
}

QString EventCreatorDlg::title()
{
    QString element = m_elements->currentText();
    QString group = m_groups->currentText();
    if (element != "")
	return element;
    return group;
}


/// ElementCreatorDlg 
ElementCreatorDlg::ElementCreatorDlg(QWidget *parent)
    : QDialog(parent)
{
    initDlg();
}

ElementCreatorDlg::~ElementCreatorDlg()
{
    delete mp_elementLabel;
    delete mp_purposeLabel;
    delete mp_noteLabel;
    delete mp_shortcutLabel;
    delete mp_unitLabel;

    delete mp_name;
    delete mp_purpose;
    delete mp_note;
    delete mp_shortcut;
    delete mp_unit;

    delete mp_okButton;

}


void ElementCreatorDlg::initDlg()
{
    // Get name
    mp_elementLabel = new QLabel(tr("Na&me:"));
    mp_name = new QLineEdit;
    mp_elementLabel->setBuddy(mp_name);

    // Get purpose
    mp_purposeLabel = new QLabel(tr("&Purpose:"));
    mp_purpose = new QSpinBox;
    mp_purpose->setMaximum(100000);
    mp_purpose->setMinimum(1);
    mp_purposeLabel->setBuddy(mp_purpose);


    // Get shortcut
    mp_shortcutLabel = new QLabel(tr("&Shortcut:"));
    mp_shortcut = new QLineEdit;
    mp_shortcutLabel->setBuddy(mp_shortcut);

    // Get unit
    mp_unitLabel = new QLabel(tr("&Unit:"));
    mp_unit = new QComboBox;
    mp_unitLabel->setBuddy(mp_unit);
    QList<QString> units = ModelAPIs::Instance()->units();
    for (int i = 0; i < units.size(); ++i){
	mp_unit->addItem(units.at(i));
    }
    
    // Get note
    mp_noteLabel = new QLabel(tr("&Note:"));
    mp_note = new QTextEdit;
    mp_noteLabel->setBuddy(mp_note);

    // Get ok/cancel button
    mp_okButton = new QDialogButtonBox;
    mp_okButton->setObjectName(QString::fromUtf8("buttonBox"));
    mp_okButton->setGeometry(QRect(30, 240, 341, 32));
    mp_okButton->setOrientation(Qt::Horizontal);
    mp_okButton->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    
    // Layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(mp_elementLabel, 0, 0);
    mainLayout->addWidget(mp_name, 0, 1);
    mainLayout->addWidget(mp_unitLabel, 1, 0);
    mainLayout->addWidget(mp_unit, 1, 1);
    mainLayout->addWidget(mp_purposeLabel, 2, 0);
    mainLayout->addWidget(mp_purpose, 2, 1);
    mainLayout->addWidget(mp_shortcutLabel, 3, 0);
    mainLayout->addWidget(mp_shortcut, 3, 1);
    mainLayout->addWidget(mp_noteLabel, 4, 0);
    mainLayout->addWidget(mp_note, 4, 1);
    mainLayout->addWidget(mp_okButton, 5, 1);
    setLayout(mainLayout);

    setWindowTitle(tr("Element Creator"));

    connect(mp_okButton, SIGNAL(accepted()), SLOT(accept()));
    connect(mp_okButton, SIGNAL(rejected()), SLOT(reject()));
}

bool ElementCreatorDlg::save()
{
    Unit* unit = ModelAPIs::unit(mp_unit->currentText());

    QString name = mp_name->text();
    if (name == "")
	return false;

    Element element;
    element.setName(name);
    element.setPurpose(mp_purpose->value());
    element.setShortcut(mp_shortcut->text());
    element.setUnit(unit);
    element.setNote(mp_note->toPlainText());
    return element.save();

}

/// GroupCreatorDlg
GroupCreatorDlg::GroupCreatorDlg(QWidget* parent)
    : QDialog(parent), m_model(new QStringListModel(this))
{
    setupUi(this);
    init();
}

void GroupCreatorDlg::init()
{
    // Init element comboBox
    QList<QString> elements = ModelAPIs::Instance()->elements();
    for (int i = 0; i < elements.size(); ++i){
	m_elements->addItem(elements.at(i));
    }

    connect(m_addButton, SIGNAL(clicked()), this, SLOT(addElementToGroupList()));

    //m_elementList, m_addButton, m_elements
    m_elementList->setModel(m_model);
    m_name->setFocus();
}

void GroupCreatorDlg::addElementToGroupList()
{
    QString element = m_elements->currentText();
    if (element != ""){
    	m_model->insertRows(m_model->rowCount(), 1);
    	m_model->setData(m_model->index(m_model->rowCount() - 1, 0), element);
    }
}

bool GroupCreatorDlg::save()
{
    //bool result = true;
    QString name = m_name->text();
    if (name == "")
	return false;
    Group* group = new Group;
    group->setName(name);
    group->setNote(m_note->toPlainText());
    if (!group->save())
	return false;
    
    // TODO: Can improve guaranty?
    QStringList elements = m_model->stringList();
    for (int i = 0; i < elements.size(); ++i){
	QString element_name = elements.at(i);

	Element* element = ModelAPIs::Instance()->element(element_name);

	ElementGroup* eg = new ElementGroup;
	eg->setElement(element);
	eg->setGroup(group);
	eg->save();
    }
    return true;
}

InputDlg::InputDlg(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    init();
}

InputDlg::~InputDlg()
{
    delete input_lineEdit;
}

void InputDlg::init()
{
    connect(input_lineEdit, SIGNAL(returnPressed()), this, SLOT(accept()));
    connect(actionEscPressed, SIGNAL(triggered()), this, SLOT(reject()));
}

// TODO: error handling for inputs. No database notification. Indicate
// where the error.
bool InputDlg::save()
{
    QString input = input_lineEdit->text();

    Record record;
    try{
	record.fromInput(input);
    }
    catch(InputError& e){
    	QMessageBox msgBox;
	QString msg = QString("It is bad input.\n") + QString("\"" + input + "\"");
    	msgBox.setText(msg);
    	msgBox.exec();
	return false;
    }

    // save
    return record.save();
}
