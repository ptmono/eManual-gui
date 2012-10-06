#include "prepare_config.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <QDebug>
#include <fstream>
using namespace std;


//A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " ")); 
    return os;
}

void Prepare_Config::initTestCase()
{
}

void Prepare_Config::test_init()
{
}

void Prepare_Config::howToUse()
{
    int argc = 3;
    char* argv[2];
    argv[0] = const_cast<char*>("gcc");
    argv[1] = const_cast<char*>("-c");
    argv[2] = const_cast<char*>("config.cfg");
    
    int opt;
    string config_file;

    // Declare a group of options that will be allowed only on command
    // line
    po::options_description generic("Generic options");
    generic.add_options()
    	("version,v", "print version string")
    	("help", "produce help message")
	("config,c", po::value<string>(&config_file)->default_value("config.cfg"),
	 "name of a file of a configuration")
    	;

    // Declare a group of options that will be allowed both on command
    // line and in config file
    po::options_description config("Configuration");
    config.add_options()
	("optimization", po::value<int>(&opt)->default_value(10),
	 "optimization level")
	("include-path,I", po::value< vector<string> >()->composing(),
	 "include path")
	("db_driver", po::value< vector<string> >(), "db_driver")
	("db_name", po::value<string>(), "db_name")
	;

    // Hidden options, will be allowed both on command line and in config
    // file, but wull not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
	("input-file", po::value< vector<string> >(), "input file")
	;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    store(po::command_line_parser(argc, argv).
	  options(cmdline_options).positional(p).run(), vm);
    notify(vm);

    //ifstream ifs(config_file.c_str());
    ifstream ifs("config.cfg");

    store(parse_config_file(ifs, config_file_options), vm);
    notify(vm);
    
    //cout << vm["db_driver"].as< vector<string> >() << "\n";
    cout << vm["db_name"].as<string>() << "\n";
    
	 
    //QCOMPARE(config_file.c_str(), "Generic options");
    //qDebug() << generic.options();

    //qDebug() << vm["config"].as<string>();

//cout << visible <<endl;
// Output
// Allowed options:

// Generic options:
//   -v [ --version ]                  print version string
//   --help                            produce help message
//   -c [ --config ] arg (=config.cfg) name of a file of a configuration

// Configuration:
//   --optimization arg (=10)  optimization level
//   -I [ --include-path ] arg include path

    

}

void Prepare_Config::cleanupTestCase()
{
}
