
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include "protob/messages.pb.h"

namespace command {

	namespace po = boost::program_options;

	struct message_command {
		Json::Value message_communication(Json::Value const &json)
		{
			wire::message wire_in;
			wire::message wire_out;	
			std::unique_ptr<core::kernel> kernel(new core::kernel());
			auto dev = kernel->enumerate_devices();
			std::unique_ptr<core::device_kernel> device(new core::device_kernel(dev[0].first.path.c_str()));

			Json::Value json_message;
			kernel->json_to_wire(json, wire_in);
			device->call(wire_in, wire_out);
			kernel->wire_to_json(wire_out, json_message);

			return json_message;
		}
	};

	struct device_command {
		
		device_command() 
		{
			if(msg.get() == nullptr) {
				msg.reset(new message_command);
			}
		}

		void help(po::options_description &desc) 
		{
			std::cout << desc << std::endl;
		}

		void version() 
		{
			std::cout << "program version is 0.1.0" << std::endl;
		}

		void list_usb() 
		{
			std::unique_ptr<core::kernel> kernel{new core::kernel};
			auto devices = kernel->enumerate_devices();
			
			if(devices.size()) {
				for(auto const &i: devices) {
					std::cout << " found device, path is : " << i.first.path << std::endl;
				}
			} else {
				LOG(INFO) << "no device found";
			}	
		}

		void test_screen(int time) 
		{
			 json_message["type"] = Json::Value("TestScreen");

			 message["delay_time"] = Json::Value(time);
			 json_message["message"] = Json::Value(message);
			 recv_json = msg->message_communication(json_message);
			 std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void get_features() 
		{
			json_message["type"] = Json::Value("Initialize");			
			recv_json = msg->message_communication(json_message);
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void set_label(std::string label)
		{
			json_message["type"] = Json::Value("ApplySettings");
			message["label"] = Json::Value(label);
			json_message["message"] = Json::Value(message);
			recv_json = msg->message_communication(json_message);
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;	
			json_message["type"] = Json::Value("ButtonAck");
			recv_json = msg->message_communication(json_message);
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}
		
		~device_command() 
		{
			msg.reset();
		}

		private :
			std::unique_ptr<message_command> msg;
			Json::Value json_message, message, recv_json;
	};
}
