#include "test_dialogs.hpp"
#include "common.hpp"

#include <QDjangoQuerySet.h>

#include "../src/dialogs.hpp"
#include "../src/models.hpp"

void TestDialogs::initTestCase()
{
    // int argc = 0;
    // char* argv[1] = {0};
    // QApplication app(argc, argv);
    // ElementCreatorDlg elementCreatorDlg;
    // elementCreatorDlg.show();
    // app.exec();
    initdb();
    dummy_db();
    

}

void TestDialogs::test_init()
{
}

void TestDialogs::cleanupTestCase()
{
}

void TestDialogs::testGui()
{
    // How can I test gui?
     // QLineEdit lineEdit;
     // QTest::keyClicks(&lineEdit, "hello world");
     // QCOMPARE(lineEdit.text(), QString("hello world"));
    cleanup_db();
}

