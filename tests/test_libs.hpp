#ifndef TEST_LIBS_HPP
#define TEST_LIBS_HPP

#include "AutoTest.h"

class TestLibs : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_stringToBool();
    
};

DECLARE_TEST(TestLibs)
#endif
