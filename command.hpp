
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>
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
			/*
			 json_msg["type"] = Json::Value("TestScreen");

			 msg["delay_time"] = Json::Value(time);
			 json_msg["message"] = Json::Value(msg);
			 recv_msg = msg_cmd->message_communication(json_msg);
			 std::cout << "recv_msg : " << recv_msg.toStyledString() << std::endl;
			 */
		}

		void get_features() 
		{
			/*
			json_msg["type"] = Json::Value("Initialize");			
			recv_msg = msg_cmd->message_communication(json_msg);
			std::cout << "recv_msg : " << recv_msg.toStyledString() << std::endl;
			*/
		}

		void set_label(std::string label)
		{
			/*
			json_msg["type"] = Json::Value("ApplySettings");
			msg["label"] = Json::Value(label);
			json_msg["message"] = Json::Value(msg);
			recv_msg = msg_cmd->message_communication(json_msg);
			if(!recv_msg["type"].asString().compare("ButtonRequest")){
				json_msg["type"] = Json::Value("ButtonAck");
				recv_msg = msg_cmd->message_communication(json_msg);
			}
			if(!recv_msg["type"].asString().compare("PinMatrixRequest")){
				std::string pin;
				std::cout << "Input Pin :";
				std::cin >> pin;	
				json_msg["type"] = Json::Value("PinMatrixAck");
				msg["pin"] = Json::Value(pin);
				json_msg["message"] = Json::Value(msg);
				recv_msg = msg_cmd->message_communication(json_msg);
			}
			std::cout << "recv_msg : " << recv_msg.toStyledString() << std::endl;
			*/
		}

		void sign_tx()
		{

			if(!json_init())
				LOG(INFO) << "parse Sign Tx json message fail";
			
			send_json_message("sign_tx");
		}
		
		~device_command() 
		{
			msg_cmd.reset();
		}

		private :
			std::unique_ptr<message_command> msg_cmd;
			Json::Value json_msg;
			Json::Reader reader;
			std::ifstream input;

			bool json_init()
			{
				input.open("./json/SignTx.json");
				assert(input.is_open());
				if(!reader.parse(input, json_msg, false)) 
					return false;

				return true;
			}

			std::string parse_receive_message(const Json::Value msg)
			{
				Json::Value type_msg, rx_msg;
				std::ostringstream req_index;
				std::string send_msg;
				
				std::cout << "recv_msg : " << msg.toStyledString() << std::endl;
				type_msg = msg["message"];
				rx_msg = type_msg["details"];
				req_index << rx_msg["request_index"].asInt();

				if(!type_msg["request_type"].asString().compare("TXINPUT")) {
					if(rx_msg["tx_hash"].type() != Json::nullValue)
						send_msg = "tx_inputs" + req_index.str();
					else
						send_msg = "txinit_inputs" + req_index.str();
				}

				if(!type_msg["request_type"].asString().compare("TXOUTPUT")) {
					if(rx_msg["tx_hash"].type() != Json::nullValue)
						send_msg = "tx_outputs" + req_index.str();
					else
						send_msg = "tx_binoutputs" + req_index.str();
				}

				if(!type_msg["request_type"].asString().compare("TXMETA")) {
					send_msg = "tx_meta" + req_index.str();
				}


				return send_msg;		
			}

			void send_json_message(const std::string &msg)
			{
				Json::Value tx_msg, recv_msg;
				std::string send_msg;
				tx_msg = json_msg[msg];
				recv_msg = msg_cmd->message_communication(tx_msg);
				send_msg = parse_receive_message(recv_msg);
				if(send_msg.empty())
					send_json_message(send_msg);
			}

	};
}
