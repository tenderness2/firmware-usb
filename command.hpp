
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <cassert>

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

			Json::Value json_msg;
			kernel->json_to_wire(json, wire_in);
			device->call(wire_in, wire_out);
			kernel->wire_to_json(wire_out, json_msg);

			return json_msg;
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
			 json_msg["type"] = Json::Value("TestScreen");

			 msg["delay_time"] = Json::Value(time);
			 json_msg["message"] = Json::Value(msg);
			 recv_json = msg_cmd->message_communication(json_msg);
			 std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void get_features() 
		{
			json_msg["type"] = Json::Value("Initialize");			
			recv_json = msg_cmd->message_communication(json_msg);
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void set_label(std::string label)
		{
			json_msg["type"] = Json::Value("ApplySettings");
			msg["label"] = Json::Value(label);
			json_msg["message"] = Json::Value(msg);
			recv_json = msg_cmd->message_communication(json_msg);
			if(!recv_json["type"].asString().compare("ButtonRequest")){
				json_msg["type"] = Json::Value("ButtonAck");
				recv_json = msg_cmd->message_communication(json_msg);
			}
			if(!recv_json["type"].asString().compare("PinMatrixRequest")){
				std::string pin;
				std::cout << "Input Pin :";
				std::cin >> pin;	
				json_msg["type"] = Json::Value("PinMatrixAck");
				msg["pin"] = Json::Value(pin);
				json_msg["message"] = Json::Value(msg);
				recv_json = msg_cmd->message_communication(json_msg);
			}
			std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
		}

		void sign_tx()
		{
			if(json_init()){
				if(send_json_message("sign_tx", "TxRequest", "")) {
					if(send_json_message("tx_ack1", "TxRequest", "TXMETA")) {
						if(send_json_message("tx_ack2", "TxRequest", "TXINPUT")){
							std::cout << "recv_json : " << recv_json.toStyledString() << std::endl;
						}
					}
				}
			}
		}
		
		~device_command() 
		{
			msg_cmd.reset();
		}

		private :

			bool json_init()
			{
				input.open("./json/SignTx.json");
				assert(input.is_open());
				if(!reader.parse(input, json_msg, false)) {
					LOG(INFO) << "parse Sign Tx json message fail";
					return false;	
				}

				return true;
			}

			void json_clear()
			{
				msg.clear();
				recv_json.clear();	
			}

			bool send_json_message(const std::string &signtx, const std::string &type, const std::string &request_type)
			{
				json_clear();
				msg = json_msg[signtx];
				recv_json = msg_cmd->message_communication(msg);
				if(!recv_json["type"].asString().compare(type))  {
					if(!request_type.empty()) {
						Json::Value rec_msg = recv_json["message"];
						if(!rec_msg["request_type"].asString().compare("TXOUTPUT")) {
							return true;
						}
					}
					return true;
				}
				return false;
			}


			std::unique_ptr<message_command> msg_cmd;
			Json::Value json_msg, msg, recv_json;
			Json::Reader reader;
			std::ifstream input;
	};
}
