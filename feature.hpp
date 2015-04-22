
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include "protob/messages.pb.h"

namespace feature {

	namespace po = boost::program_options;

	struct device_feature {

		device_feature(){ hid::init(); };
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
			std::unique_ptr<core::device_kernel> device(new core::device_kernel(path.c_str())); 
			//std::cout << "path : " << path << std::endl;
			device->open();
			std::cout << "is opened !" << std::endl;
		}

		void test_screen(int time) {
			 std::unique_ptr<core::kernel> kernel(new core::kernel());
			 Json::Value json_message;
			 json_message["type"] = Json::Value("TestScreen");

			 Json::Value message;
			 message["delay_time"] = Json::Value(time);

			 json_message["message"] = Json::Value(message);

			 wire::message wire_in;
			 wire::message wire_out;
			 std::unique_ptr<core::device_kernel> device(new core::device_kernel("0002:003c:00")); 
			 device->open();
			//std::cout << "path : " << path << std::endl;
			
			 kernel->json_to_wire(json_message, wire_in);
			 device->call(wire_in, wire_out);
			 kernel->wire_to_json(wire_out, json_message);

		}

		~device_feature(){ hid::exit(); };

		private :
			wire::message wire_in;
			wire::message wire_out;	
			using protobuf_ptr = std::unique_ptr<protobuf::pb::Message>;
			
	};
}
