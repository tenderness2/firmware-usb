
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
			std::cout << "bwallet offline tools version 0.1.1" << std::endl;
		}

		void list_usb() 
		{
			try {
				std::unique_ptr<core::kernel> kernel{new core::kernel};
				auto devices = kernel->enumerate_devices();

				if(devices.size()) {
					for(auto const &i: devices) {
						std::cout << " found device, path is : " << i.first.path << std::endl;
					}
				} else {
					LOG(INFO) << "no device found";
				}	

			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void test_screen(const std::string &cmd_json) 
		{
			try {
				Json::Value msg;
				msg = find_command_message(cmd_json);
				std::cout << "testscreen_msg : " << msg.toStyledString() << std::endl;
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void get_features(const std::string &cmd_json) 
		{
			try {
				Json::Value msg;
				msg = find_command_message(cmd_json);
				std::cout << "initialize_msg : " << msg.toStyledString() << std::endl;

			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void set_label(const std::string &cmd_json)
		{
			try {
				Json::Value msg;
				msg = find_command_message(cmd_json);
				std::cout << "setlabel_msg : " << msg.toStyledString() << std::endl;
				/*
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

			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void sign_tx(const std::string &cmd_json)
		{
			try {
				Json::Value msg;
				std::string send_msg;

				msg = find_command_message(cmd_json);
				send_msg = parse_signtx_message(msg);
				while(!send_msg.empty()) {
					msg.clear();
					msg = send_json_message(send_msg);
					std::cout << "signtx_msg porecss : " << msg.toStyledString() << std::endl;
					send_msg = parse_signtx_message(msg);
				}
				std::cout << "signtx_msg : " << msg.toStyledString() << std::endl;
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
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

		bool json_init(const std::string &cmd_json)
		{
			std::string path;
			path = "./json/" + cmd_json + ".json";
			input.open(path);
			assert(input.is_open());
			if(!reader.parse(input, json_msg, false)) 
				return false;

			return true;
		}

		Json::Value find_command_message(const std::string &find_msg)
		{
			Json::Value msg;
			if(!json_init(find_msg))
				LOG(INFO) << "parse Sign Tx json message fail";

			msg = send_json_message(find_msg);
			if(!msg["type"].asString().compare("ButtonRequest")){
				msg.clear();
				send_json_message("button_ack");
			}

			return msg;
		}

		std::string parse_signtx_message(const Json::Value &msg)
		{
			Json::Value type_msg, details_msg;
			std::ostringstream req_index;
			std::string send_msg;

			type_msg = msg["message"];
			details_msg = type_msg["details"];
			req_index << details_msg["request_index"].asInt();

			if(!msg["type"].asString().compare("TxRequest")) {
				if(!type_msg["request_type"].asString().compare("TXINPUT")) {
					if(details_msg["tx_hash"].type() != Json::nullValue)
						send_msg = "tx_inputs" + req_index.str();
					else
						send_msg = "txinit_inputs" + req_index.str();
				}

				if(!type_msg["request_type"].asString().compare("TXOUTPUT")) {
					if(details_msg["tx_hash"].type() == Json::nullValue)
						send_msg = "tx_outputs" + req_index.str();
					else
						send_msg = "txbin_outputs" + req_index.str();
				}

				if(!type_msg["request_type"].asString().compare("TXMETA")) {
					send_msg = "tx_meta" + req_index.str();
				}

				return send_msg;		
			}

			if(!msg["type"].asString().compare("ButtonRequest")) {
				send_msg = "button_ack";
				return send_msg;				
			}

			return nullptr;

		}

		Json::Value send_json_message(const std::string &msg)
		{
			Json::Value tx_msg, recv_msg;
			tx_msg = json_msg[msg];
			recv_msg = msg_cmd->message_communication(tx_msg);

			return recv_msg;
		}
	};
}
