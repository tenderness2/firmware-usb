
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
//#include "protob/messages.pb.h"
//#include "protob/config.pb.h"


namespace command {

	namespace po = boost::program_options;

	struct message_command {
		Json::Value message_communication(Json::Value const &json)
		{
			wire::message wire_in;
			wire::message wire_out;	
			std::unique_ptr<core::kernel> kernel(new core::kernel());
			//kernel->set_config();
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
			if(msg_cmd.get() == nullptr) {
				msg_cmd.reset(new message_command);
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

			 msg["delay_time"] = Json::Value(time);
			 json_message["message"] = Json::Value(msg);
			 recv_json = msg_cmd->message_communication(json_message);
			 std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void get_features() 
		{
			json_message["type"] = Json::Value("Initialize");			
			recv_json = msg_cmd->message_communication(json_message);
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void set_label(std::string label)
		{
			json_message["type"] = Json::Value("ApplySettings");
			msg["label"] = Json::Value(label);
			json_message["message"] = Json::Value(msg);
			recv_json = msg_cmd->message_communication(json_message);
			if(!recv_json["type"].asString().compare("ButtonRequest")){
				json_message["type"] = Json::Value("ButtonAck");
				recv_json = msg_cmd->message_communication(json_message);
			}
			if(!recv_json["type"].asString().compare("PinMatrixRequest")){
				std::string pin;
				std::cout << "Input Pin :";
				std::cin >> pin;	
				json_message["type"] = Json::Value("PinMatrixAck");
				msg["pin"] = Json::Value(pin);
				json_message["message"] = Json::Value(msg);
				recv_json = msg_cmd->message_communication(json_message);
			}
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void sign_tx()
		{
			json_message["type"] = Json::Value("SignTx");
			msg["outputs_count"] = Json::Value(2);
			msg["inputs_count"] = Json::Value(1);
			//msg["has_coin_name"] = Json::Value(true);
			msg["coin_name"] = Json::Value("Bitcoin");
			json_message["message"] = Json::Value(msg);
			recv_json = msg_cmd->message_communication(json_message);
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
			if(!recv_json["type"].asString().compare("TxRequest")) {
				json_message["type"] = Json::Value("SignTx");
				msg["inputs"] = Json::Value(1);
				msg["prev_hash"] = Json::Value("44bd70d901b76dfc07bc76eede4b0a0f28e145d5f9baf27f3ee414e87bf1ace5");
				msg["prev_index"] = Json::Value(1);
				//msg["add"]
				
			}
			
		}
		
		~device_command() 
		{
			msg_cmd.reset();
		}

		private :
			std::unique_ptr<message_command> msg_cmd;
			Json::Value json_message, msg, recv_json;
	};
}
