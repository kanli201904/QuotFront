#include"ConfigManager.h"
#include "log.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/make_shared.hpp>

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("QuoteFront.ConfigManager");

BaseConfig::BaseConfig()
{
}

BaseConfig::~BaseConfig()
{
}

void BaseConfig::set_config_file(const std::string &file_path)
{
    if (0 == file_path.compare(config_file)
		  || file_path.length() == 0) {
        return;
    }
    this->config_file = file_path;
    
	try 
    {
        boost::property_tree::json_parser::read_json(this->config_file, config_tree);
        parse_config_file();
    }
    catch(std::exception &e)
    {
		printf("Read json file exception!\n");
		if (nullptr != logger) 
		{
			logger->error("load config file {} error: {}", this->config_file, e.what());
		}
        return;
    }
}

void BaseConfig::parse_config_file()
{
    return;
}

template<typename T>
void BaseConfig::register_variable(const std::string & name, const std::string & conf_path, T  default_value)
{
	variable_map[name] = conf_path;
	default_map[name] = default_value;
}

InstrumentConfig::InstrumentConfig() :instrument_map()
{
}

InstrumentConfig::~InstrumentConfig()
{
    instrument_map.clear();
}

void InstrumentConfig::parse_config_file()
{
    try
    {
        if (config_tree.empty()) 
        {
			printf("InstrumentConfig::config_tree empty");
            return;
        }

        boost::property_tree::ptree &instrument_tree = config_tree.get_child("instruments");
        for (boost::property_tree::ptree::value_type &v : instrument_tree) 
		{
            unsigned int id = v.second.get<unsigned int>("ID");
			std::stringstream com_ins;
               com_ins << v.second.get<std::string>("Exchange") 
				   << "@"
				   << v.second.get<std::string>("Code") 
				   << "."
				   << v.second.get<std::string>("Country","US");

            instrument_map.emplace(std::pair< std::string,unsigned int>(com_ins.str(),id));
        }
    }
    catch (std::exception &e)
    {
		printf("InstrumentConfig::exception");
		/*if (nullptr != logger) 
		{
			logger->error("Resolver config file {} error: {}", config_file, e.what());
		}*/
        return;
    }
}

template<typename T>
void ConfigManager::getConfig(const std::string & config_file, boost::shared_ptr<T> &p_conf)
{
	boost::unique_lock<boost::mutex> lock(conf_handler_mutex);
	if(p_conf == nullptr)
	{
		p_conf = boost::make_shared<T>();
		p_conf->set_config_file(config_file);
	}
}

ConfigManager::ConfigManager()
{

}

boost::shared_ptr<InstrumentConfig> ConfigManager::getInstrumentConfig()
{
	if (p_Ins == nullptr)
	{
		getConfig<InstrumentConfig>(INSTRUMENT_CONFIG_FILE, p_Ins);
	}
	return p_Ins;
}

boost::shared_ptr<BaseConfig> ConfigManager::getQuoteConfig()
{
	if (p_conf == nullptr)
	{
		getConfig<BaseConfig>(QUOTE_CONFIG_FILE, p_conf);

		p_conf->register_variable < std::string > ("zmq_address", "zmq.zmq_address", "epgm://192.168.80.20;239.0.0.1:5555");
		p_conf->register_variable<int>("zmq_rate", "zmq.zmq_rate", 100000);
		p_conf->register_variable<int>("zmq_rcvbuf", "zmq.zmq_rcvbuf", 20000000);
		p_conf->register_variable<int>("zmq_rcvhwm", "zmq.zmq_rcvhwm", 9999999);
		p_conf->register_variable<int>("zmq_recover", "zmq.zmq_recover", 10000);

		p_conf->register_variable<int>("logLevel", "log.logLevel", 1);
		p_conf->register_variable<std::string>("logPath", "log.logPath", "./log");
		p_conf->register_variable<int>("hour", "log.hour", 2);
		p_conf->register_variable<int>("min", "log.min", 30);
		p_conf->register_variable<int>("port", "tcpserver.port", 11700);
		p_conf->register_variable<int>("maxconnect", "tcpserver.maxconnect", 1000);
		p_conf->register_variable<int>("cirbuffmaxsize", "application.cirbuffmaxsize", 150);
		p_conf->register_variable<std::string>("loginuser", "application.loginuser", "zd987#");
	}
	return p_conf;
}

