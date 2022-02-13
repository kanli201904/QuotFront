#pragma once
#include <boost/filesystem.hpp>
#include <boost/serialization/singleton.hpp>
#include <iostream>
#include <memory>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>

#include "ConfigManager.h"

/**
 * 目录下需要有log.json配置文件
 * 目前性能配置线程1，日志的大小4096，刷新周期1s。优化的话可以进行具体修改，目前配置可以通过配置日志级别，日志目录，每天生成日志的时间（hh:mm）
 * trace = 0,
 * debug = 1,
 * info = 2,
 * warn = 3,
 * err = 4,
 * critical = 5,
 * off = 6
 *Date :[11/27/2018 ]
 *Author :[RS]
 */
typedef std::shared_ptr<spdlog::logger> LOGGER;
class LoggerUtils :public boost::serialization::singleton<LoggerUtils> {
public:
	LoggerUtils() {
		try {
			logconf.logLevel = GET_CFG->get_value<int>("logLevel");
			logconf.logPath = GET_CFG->get_value<std::string>("logPath");
			logconf.hour = GET_CFG->get_value<int>("hour");
			logconf.min = GET_CFG->get_value<int>("min");
		}
		catch (const std::exception & e) {
			std::cout << "cant read config file ./log.json :" << e.what() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			exit(1);
		}
		boost::system::error_code ec;
		boost::filesystem::create_directories(logconf.logPath, ec);
		if (ec.value() != 0) {
			std::cout << ec.message() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			exit(1);
		}
		logconf.logPath += "/Zdlog.txt";
		spdlog::init_thread_pool(4096, 1);
		spdlog::flush_every(std::chrono::seconds(1));
		sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
		try {
			sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logconf.logPath, logconf.hour, logconf.min));
		}
		catch (const std::exception & e) {
			std::cout << "sinks push error:" << e.what() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			exit(1);
		}

		loglevel = spdlog::level::level_enum(logconf.logLevel);
	}
	~LoggerUtils() {
		spdlog::drop_all();
	}

	/**
	 *异步日志注册
	 * 注意不要重复注册
	 *Date :[11/27/2018 ]
	 *Author :[RS]
	 */
	LOGGER getLogger(const char* loggername) {
		auto logTmp = spdlog::get(loggername);
		if (logTmp != nullptr) {
			return logTmp;
		}
		auto logger = std::make_shared<spdlog::async_logger>(loggername, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
		logger->set_level(loglevel);
		spdlog::register_logger(logger);
		return logger;
	}
private:
	/**
	 *日志依赖 boost,json,spdlog 三个库，其中json,spdlog是only header 类型的库。
	 *boost主要处理目录生成以及单例，json配置文件解析，spdlog为三方日志库
	 *Date :[12/11/2018 ]
	 *Author :[RS]
	 */
	struct LogConf {
		int logLevel;
		std::string logPath;
		int hour;
		int min;
	} ;

	spdlog::level::level_enum loglevel;
	LogConf logconf;
	std::vector<spdlog::sink_ptr> sinks;
};
