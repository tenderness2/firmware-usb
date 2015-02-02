/*******************************************************
 *  BWallet usb communication
 *
 *  kevin 
 *  Bidingxing Software
 *     
 *  01/12/2015
 *
 *  Copyright 2015
 *        
 *********************************************************/

#include <iostream>
#include <stdio.h>
#include <hidapi/hidapi.h>
#include "easylogging++.h"
#include <boost/program_options.hpp>
#include <boost/chrono/chrono.hpp>

_INITIALIZE_EASYLOGGINGPP


void configure_logging()
{
	const auto default_format = "%datetime %level [%logger] %msg";
	const auto max_log_file_size = "2097152"; // 2mb, log gets truncated after that

	el::Configurations cfg;
	cfg.setToDefault();
	cfg.setGlobally(el::ConfigurationType::MaxLogFileSize, max_log_file_size);
	cfg.setGlobally(el::ConfigurationType::Format, default_format);
	cfg.set(el::Level::Debug, el::ConfigurationType::Format, default_format);
	cfg.set(el::Level::Trace, el::ConfigurationType::Format, default_format);
	cfg.set(el::Level::Verbose, el::ConfigurationType::Format, default_format);

	//el::Loggers::getLogger("core.device");
	//el::Loggers::getLogger("core.config");
	//el::Loggers::getLogger("core.kernel");
	//el::Loggers::getLogger("wire.enumerate");

	el::Loggers::reconfigureAllLoggers(cfg);
	//LOG(INFO) << "this is test";

}

int main(int argc, char **argv)
{
	configure_logging();

	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "program help message")
		("version,v", "program version")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help") || (argc == 1)) {
		std::cout << desc << std::endl;
		return 1;
	}

	if(vm.count("version")) {
		std::cout << "program version is 0.1.0" << std::endl;
		return 1;
	}

	return 0;
}

