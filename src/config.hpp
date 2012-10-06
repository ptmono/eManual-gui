#ifndef CONFIG_HPP
#define CONFIG_HPP

//#define DB_DRIVER "QSQLITE"
//#define DB_NAME ":memory:"

#include <QString>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;


// TODO: create config.cfg if not exist.
/** 
 * Used to get configuration. It is used as a singleton. get methods
 * returns the configuration values.
 *
 * @return 
 */
class Config
{
public:
    static Config* Instance();
    // TODO: I want to a method to change the name of configuration file.
    //static Config* Instance(const string& filename);
    string getDbName() const;
    string getDbDriver() const;
    string getTestDbName() const;
    /** 
     * The function will used to when we want to reload the content of the
     * config file. For instance, we had been edit config file, then we
     * want to update configuration of app. Config singleton have to
     * reload the configuration file.
     *
     */
    static void Delete();

private:
    Config(){};
    Config(Config const&){};
    Config& operator=(Config const& obj);

    void setVm();
    static Config* m_pInstance;
    static string m_cfg_filename;
    static po::options_description* m_config;
    static po::variables_map m_vm;
};

#endif

