#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/serialization/singleton.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/enable_shared_from_this.hpp>

#define INSTRUMENT_CONFIG_FILE "./config/instruments.json"
#define QUOTE_CONFIG_FILE "./config/quote_front.json"

#define GET_INSTRUMENT_CFG ConfigManager::get_mutable_instance().getInstrumentConfig()
#define GET_CFG ConfigManager::get_mutable_instance().getQuoteConfig()

class BaseConfig
{
public:
	BaseConfig();
	virtual ~BaseConfig();

	template<typename T>
	void register_variable(const std::string & name, const std::string & conf_path, T default_value);

	void set_config_file(const std::string &file_path);
	
	//第二参数仅用于辅助类型推导
	template<typename T>
	T get_value(const std::string& name)
	{
		auto var_it = variable_map.find(name);
		if (var_it != variable_map.end())
		{
			try {
				return config_tree.get<T>(var_it->second);

			}
			catch (std::exception &e) {
				std::cout <<"Node Name:"<<name<< "  exception:"<< e.what();
			}
			return boost::any_cast<T>(default_map.find(name)->second);
		}

		throw std::runtime_error(std::string("Get variable " + name + " from file " + config_file + " error , undefined variable"));
	}
	template<typename T>
	void set_value(const std::string& name, T& value)
	{
		auto var_it = variable_map.find(name);
		if (var_it != variable_map.end())
		{
			try
			{
				config_tree.put(var_it->second, value);
				boost::property_tree::json_parser::write_json(this->config_file, config_tree);
			}
			catch (std::exception& e)
			{
				//log.error("Set config data error: {}", e.what());
			}
		}

		throw std::runtime_error(std::string("Set variable " + name + " to file " + config_file + " error , unregistered variable"));
	}
protected:
	virtual void parse_config_file();
	std::string config_file;

	boost::shared_mutex config_mutex;
	boost::property_tree::ptree config_tree;
private:
	std::map<std::string, std::string> variable_map;
	std::map<std::string, boost::any> default_map;
};

class InstrumentConfig : public BaseConfig
{
public:
	InstrumentConfig();
	~InstrumentConfig();

	std::unordered_map<std::string, unsigned int> instrument_map;

protected:
	void parse_config_file();
};

class ConfigManager : public boost::serialization::singleton<ConfigManager>
{
public:
	ConfigManager();
	//合约配置
	boost::shared_ptr<InstrumentConfig> getInstrumentConfig();
	//主配置
	boost::shared_ptr<BaseConfig> getQuoteConfig();
private:	
	boost::mutex conf_handler_mutex;
	template<typename T>
	void getConfig(const std::string &config_file, boost::shared_ptr<T>& p_conf);

	boost::shared_ptr<InstrumentConfig> p_Ins;
	boost::shared_ptr<BaseConfig> p_conf;
};