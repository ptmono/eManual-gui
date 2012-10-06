#ifndef TEST_DIALOGS_HPP
#define TEST_DIALOGS_HPP

#include "AutoTest.h"

class TestDialogs : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_init();
    void testGui();
    void cleanupTestCase();
};

DECLARE_TEST(TestDialogs)
#endif
