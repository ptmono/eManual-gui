#include "test_config.hpp"
#include "../src/config.hpp"

#include <cstdio>
#include <iostream>
#include <fstream>		// ifstream, ofstream

#include <QTest>
#include <QDebug>

#include <boost/filesystem.hpp>

using namespace std;

static string config_filename = "config.cfg";
static string file_content = "db_name=:memory:\ndb_driver=QSQLITE";

void TestConfig::initTestCase()
{
}

void TestConfig::init()
{
    // Create configuration file
    ofstream ofs(config_filename.c_str());
    ofs << file_content <<endl;
}


void TestConfig::cleanupTestCase()
{
    // Remove created database file.
    remove(Config::Instance()->getDbName().c_str());
}

void TestConfig::cleanup()
{
    remove(config_filename.c_str());    
}

void TestConfig::test_Config_basic()
{
    QVERIFY(Config::Instance()->getDbName() == ":memory:");
    QVERIFY(Config::Instance()->getDbDriver() == "QSQLITE");
}

/** 
 * Config::Delete() will used to when we want to reload the content of
 * config file.
 *
 */
void TestConfig::test_Config_Delete()
{

    //// Let's change the content of config file.
    string new_file_content = "db_name=test_db.sqlite\ndb_driver=QSQLITE";
    // RAII will flush the file. Or use close.
    {
	ofstream ofs(config_filename.c_str());
	ofs << new_file_content;
    }

    // Check the content of config file
    ifstream ifs(config_filename.c_str());
    stringstream buffer;
    string content;
    buffer << ifs.rdbuf();
    content = buffer.str();

    QVERIFY(content.size() == new_file_content.size());
    QVERIFY(content == new_file_content);

    QVERIFY(Config::Instance()->getDbName() == ":memory:");
    Config::Delete();
    QVERIFY(Config::Instance()->getDbName() == "test_db.sqlite");
}

