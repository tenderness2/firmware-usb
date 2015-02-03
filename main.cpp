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
#include "easylogging++.h"
#include <boost/program_options.hpp>
#include <boost/chrono/chrono.hpp>
#include <vector>
#include "wire.hpp"
#include "core.hpp"

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

	el::Loggers::getLogger("core.device");
	el::Loggers::getLogger("wire.enumerate");

	el::Loggers::reconfigureAllLoggers(cfg);

}

int main(int argc, char **argv)
{
	configure_logging();

	try {
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "program help message")
			("version,v", "program version")
			("list,l", "display hid usb device")
			("path,p", po::value<std::string>(), "set usb path")
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

		if(vm.count("list")) {
			wire::device_info_list list;
			list = wire::enumerate_connected_devices(0x534c, 0x0001);

			if(list.size())
			{	
				std::vector<wire::device_info>::iterator iter;
				for(iter = list.begin(); iter != list.end(); iter++) {
					std::cout << "path : " << iter->path << std::endl;	
					std::wcout << "serial number: " <<  iter->serial_number << std::endl;	
				}
			}
			else
			{
				LOG(INFO) << "no device found";
			}
		}

		if(vm.count("path")) {
			auto path = vm["path"].as<std::string>();
			std::cout << "path : " << path << std::endl;
			core::device_kernel device(path);	
			device.open();
		}
	} catch(std::exception& e) {
		LOG(ERROR) << e.what();
	} catch(...) {
		LOG(ERROR) << "Unknown Error";
	}


	return 0;
}

