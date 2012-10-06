#ifndef TEST_CONFIG_HPP
#define TEST_CONFIG_HPP

#include "AutoTest.h"

class TestConfig : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void test_Config_basic();
    void test_Config_Delete();
    void cleanupTestCase();
    void cleanup();
};

DECLARE_TEST(TestConfig)
#endif
