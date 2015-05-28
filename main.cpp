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
#include <boost/program_options.hpp>
#include <boost/chrono/chrono.hpp>
#include <vector>
#include "easylogging++.h"
#include "hid.hpp"
#include "utils.hpp"
#include "wire.hpp"
#include "wire_proto.hpp"
#include "json_codec.hpp"
#include "core.hpp"
#include "command.hpp"

static const auto sleep_time = boost::chrono::seconds(10);

_INITIALIZE_EASYLOGGINGPP

void configure_logging()
{
	const auto default_format = "%datetime %level [%logger] %msg";
	const auto max_log_file_size = "2097152"; // 2mb, log gets truncated after that
	const auto log_path = "/var/log/myfirmware.log";

	el::Configurations cfg;
	cfg.setToDefault();
	cfg.setGlobally(el::ConfigurationType::MaxLogFileSize, max_log_file_size);
	cfg.setGlobally(el::ConfigurationType::Format, default_format);
	cfg.setGlobally(el::ConfigurationType::Filename,log_path);
	cfg.set(el::Level::Debug, el::ConfigurationType::Format, default_format);
	cfg.set(el::Level::Trace, el::ConfigurationType::Format, default_format);
	cfg.set(el::Level::Verbose, el::ConfigurationType::Format, default_format);

	el::Loggers::getLogger("core.device");
	el::Loggers::getLogger("core.kernel");
	el::Loggers::getLogger("wire.enumerate");

	el::Loggers::reconfigureAllLoggers(cfg);

}

int main(int argc, char **argv)
{
	configure_logging();

	try {
		namespace po = boost::program_options;
		using namespace hid;
		using namespace wire;
		std::unique_ptr<command::device_command>  cmd{new command::device_command};

		po::options_description desc("Options");
		desc.add_options()
			("help,h", "program help message")
			("version,v", "program version")
			("list,l", "display hid usb device")
			("test_screen,t", po::value<std::string>(), "test home screen")
			("get_features", po::value<std::string>(),"Retrieve device features and settings")
			("set_label", po::value<std::string>(), "Set new wallet label")
			("sign_tx,s", po::value<std::string>(), "Sign Bitcoin Tx")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if(vm.count("help") || (argc == 1)) {
			cmd->help(desc);
			return 1;
		}

		if(vm.count("version")) {
			cmd->version();
			return 1;
		}

		if(vm.count("list")) {
			cmd->list_usb();
			return 1;
		}
		
		if(vm.count("test_screen")) {
			auto  cmd_json = vm["test_screen"].as<std::string>();	
			cmd->test_screen(cmd_json);
			return 1;
		}

		if(vm.count("get_features")) {
			auto  cmd_json = vm["get_features"].as<std::string>();	
			cmd->get_features(cmd_json);
			return 1;
		}

		if(vm.count("set_label")) {
			auto cmd_json = vm["set_label"].as<std::string>();
			cmd->set_label(cmd_json);
			return 1;
		}
		
		if(vm.count("sign_tx"))
		{
			auto cmd_json = vm["sign_tx"].as<std::string>();
			cmd->sign_tx(cmd_json);
			return 1;
		}
	} catch(std::exception& e) {
		LOG(ERROR) << e.what();
	} catch(...) {
		LOG(ERROR) << "Unknown Error";
	}


	return 0;
}

