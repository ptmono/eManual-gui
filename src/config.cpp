#include "config.hpp"

#include <iostream>
#include <fstream>

using namespace std;

// It is convenient to debug. po uses vector<T>.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " ")); 
    return os;
}

Config* Config::m_pInstance = NULL;
po::variables_map Config::m_vm = NULL;


Config* Config::Instance()
{
    if (!m_pInstance){
	m_pInstance = new Config();
	m_pInstance->setVm();
    }
    return m_pInstance;
}

void Config::Delete()
{
    if (m_pInstance){
	delete m_pInstance;
	delete m_config;
	m_pInstance = NULL;
	m_config = NULL;
	m_vm = NULL;
    }
}


// Config* Config::Instance(const string& cfg_filename)
// {
//     if (!m_pInstance){
// 	// Init Config
// 	m_cfg_
	    
    
// }


string Config::getTestDbName() const
{
    return m_vm["testdb_name"].as<string>();
}

string Config::getDbName() const
{
    return m_vm["db_name"].as<string>();
}

string Config::getDbDriver() const
{
    return m_vm["db_driver"].as<string>();
}

string Config::m_cfg_filename = "config.cfg";
po::options_description* Config::m_config = NULL;

void Config::setVm()
{
    m_config = new po::options_description("Configuration");
    m_config->add_options()
	("config_filename", po::value<string>(&m_cfg_filename)->default_value("config.cfg"))
	("db_driver", po::value<string>()->default_value("QSQLITE"), "database driver")
	("db_name", po::value<string>()->default_value(":memory:"), "database file name")
	("testdb_name", po::value<string>()->default_value("testdb.sqlite"), "database file name")
	;

    ifstream ifs(m_cfg_filename.c_str());
    po::store(parse_config_file(ifs, *m_config), m_vm);
    notify(m_vm);
}
