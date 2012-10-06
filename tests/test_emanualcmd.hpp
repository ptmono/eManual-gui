#ifndef TEST_EMANUALCMD_HPP
#define TEST_EMANUALCMD_HPP

#include "AutoTest.h"

class TestEmanualCmd : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_gui();
    void cleanupTestCase();

    void test_EmanualCmd_editSchedule();
    void test_EmanualCmd_getEvents();
};

DECLARE_TEST(TestEmanualCmd)
#endif
