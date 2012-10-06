/**
 * @file   main.cpp
 * @author ptmono <ptmono@gmail.com>
 * @date   Tue Feb 28 17:07:34 2012
 * 
 * @brief  test
 * 
 * 
 */

#include <AutoTest.h>
#include <QApplication>
#include <QDebug>
#include <QtGui/QPushButton>

#include "../src/mainwindow.hpp"

#include "../src/dialogs.hpp"

#if 0
// This is all you need to run all the tests
TEST_MAIN

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     //ElementCreatorDlg elementCreatorDlg;
//     //elementCreatorDlg.show();
//     EventCreatorDlg eventCreatorDlg;
//     eventCreatorDlg.show();
//     return app.exec();
// }

#else

// Or supply your own main function
/** 
 * what is this
 *
 * @param argc 
 * @param argv 
 *
 * @return 
 */
int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWin win;
    win.show();
    return app.exec();

    int failures = AutoTest::run(argc, argv);
    if (failures == 0) {
	qDebug() << "ALL TESTS PASSED";
    } else {
	qDebug() << failures << " TESTS FAILED!";
	QPushButton quit("Quit");
    }
    return failures;
}
#endif

