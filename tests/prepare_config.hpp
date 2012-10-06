#ifndef PREPARE_CONFIG_HPP
#define PREPARE_CONFIG_HPP

#include "AutoTest.h"

class Prepare_Config : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_init();
    void howToUse();
    void cleanupTestCase();
};

DECLARE_TEST(Prepare_Config)
#endif
