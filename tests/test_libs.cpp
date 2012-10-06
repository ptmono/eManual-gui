#include "test_libs.hpp"

#include <QDebug>
#include "../src/libs.hpp"


using namespace std;

void TestLibs::initTestCase()
{
}

void TestLibs::cleanupTestCase()
{
}

void TestLibs::test_stringToBool()
{
    string aa = "true";
    string bb = "false";

    bool result = stringToBool(aa);
    QVERIFY(result == true);

    bool result2 = stringToBool(bb);
    QVERIFY(result2 == false);
}
