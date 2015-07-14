
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include "common_msg.hpp"

namespace command {

	namespace po = boost::program_options;
	struct device_command {

		void help(po::options_description &desc) 
		{
			std::cout << desc << std::endl;
		}

		void version() 
		{
			std::cout << "bwallet offline tools version 0.1.1" << std::endl;
		}

		void list_usb() 
		{
			try {
				std::unique_ptr<core::kernel> kernel{new core::kernel};
				auto devices = kernel->enumerate_devices();

				if(devices.size()) {
					for(auto const &i: devices) {
						std::cout << "found device, path is : " << i.first.path << std::endl;
					}
				} else {
					LOG(INFO) << "no device found";
				}	

			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void test_screen(std::string const &cmd) 
		{
			try {
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};

				auto read_msg = common_msg::message_communication(json_msg[cmd]);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void get_features(std::string const &cmd) 
		{
			try {
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};
				auto read_msg = common_msg::message_communication(json_msg[cmd]);
				std::cout << "read_msg : " << read_msg.toStyledString() << std::endl;
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void set_label(std::string const &cmd)
		{
			try {
				if(!json_init(cmd)) 
					throw std::runtime_error{"parse json message fail"};

				auto read_msg = common_msg::message_communication(json_msg[cmd]);
				common_msg::parse_read_message(read_msg);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void get_address(std::string const &cmd)
		{
			try {
				if(!json_init(cmd)) 
					throw std::runtime_error{"parse json message fail"};

				auto read_msg = common_msg::message_communication(json_msg[cmd]);
				common_msg::parse_read_message(read_msg);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void sign_tx(std::string const &cmd)
		{
			try {

				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};

				auto url = json_msg["url"].asString() + json_msg["prev_hash"].asString();	
				common_msg::http_client_request_data(url);

				common_msg::write_signtx_init(json_msg);

			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		private :
			Json::Value json_msg ;
			Json::Reader reader;
			std::ifstream input;

		bool json_init(std::string const &json_cfg)
		{
			std::string path;
			path = "./json/" + json_cfg + ".json";
			input.open(path);
			assert(input.is_open());
			if(!reader.parse(input, json_msg, false)) {
				return false;	
			}

			return true;
		}
	};
}
