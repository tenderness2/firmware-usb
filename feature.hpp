
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include "protob/messages.pb.h"

namespace feature {

	namespace po = boost::program_options;

	struct message_feature {

		message_feature(std::string const &dp) : device_path(dp) {}

		Json::Value message_communication(Json::Value const &json)
		{
			wire::message wire_in;
			wire::message wire_out;	
			std::unique_ptr<core::kernel> kernel(new core::kernel());
			std::unique_ptr<core::device_kernel> device(new core::device_kernel(device_path.c_str()));
			//core::device_kernel *device;

			//device->open();
			Json::Value json_message;
			kernel->json_to_wire(json, wire_in);
			device->call(wire_in, wire_out);
			kernel->wire_to_json(wire_out, json_message);

			return json_message;
		}

		private :
			std::string device_path;

	};

	struct device_feature {

		device_feature() { hid::init();}

		void help(po::options_description &desc) {
			std::cout << desc << std::endl;
		}

		void version() {
			std::cout << "program version is 0.1.0" << std::endl;
		}

		void list_usb() {
			wire::device_info_list list;
			list = wire::enumerate_connected_devices();
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
		
		void device_path(std::string path) {
			msg.reset(new message_feature(path));
		}

		void test_screen(int time) {
			 Json::Value json_message, recv_json;
			 json_message["type"] = Json::Value("TestScreen");

			 Json::Value message;
			 message["delay_time"] = Json::Value(time);
			 json_message["message"] = Json::Value(message);
			 recv_json = msg->message_communication(json_message);
			 std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		~device_feature(){ hid::exit(); };

		private :
			std::unique_ptr<message_feature> msg;	
	};
}
