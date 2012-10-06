#ifndef TEST_MODELS_HPP
#define TEST_MODELS_HPP

#include "AutoTest.h"		// <QTest>, <QList>, <QString> <QSharedPointer>
				// QCOMPARE

class TestModels : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_common();
    void cleanupTestCase();

    void test_Schedule();

    void test_Record();
    void test_RecordSchedule();

    void test_ModelAPIs_units();
    void test_ModelAPIs_schedules();
    void test_ModelAPIs_elements();
    void test_ModelAPIs_groups();
    void test_ModelAPIs_unit();
    void test_ModelAPIs_schedule();
    void test_ModelAPIs_lastSchedule();
    void test_ModelAPIs_elementFromShortcut();
    void test_ModelAPIs_currentSchedule();
};
DECLARE_TEST(TestModels)

#endif
