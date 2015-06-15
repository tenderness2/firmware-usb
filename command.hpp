
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include "http_client.hpp"

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

		void test_screen(std::string const &cmd) 
		{
			try {
				Json::Value msg;
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};
				msg = write_json_message(cmd);
				std::cout << "testscreen_msg : " << msg.toStyledString() << std::endl;
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void get_features(std::string const &cmd) 
		{
			try {
				Json::Value msg;
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};
				msg = write_json_message(cmd);
				std::cout << "initialize_msg : " << msg.toStyledString() << std::endl;

			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void set_label(std::string const &cmd)
		{
			try {
				Json::Value msg, pin_msg;
				if(!json_init(cmd)) 
					throw std::runtime_error{"parse json message fail"};

				msg = write_json_message(cmd);
				std::cout << "setlabel_msg : " << msg.toStyledString() << std::endl;
				if(!msg["type"].asString().compare("ButtonRequest")){
					msg.clear();
					msg = write_json_message("button_ack");
					if(!msg["type"].asString().compare("PinMatrixRequest")){
						msg = pin_request();
						std::cout << "read_msg : " << msg.toStyledString() << std::endl;
					}
				}
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void sign_tx(std::string const &cmd)
		{
			try {
				Json::Value url_data;
				Json::Reader stream;

				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};

				auto url = json_msg[cmd]["url"].asString() + json_msg[cmd]["prev_hash"].asString();	
				auto url_stream = http_client::request_url_to_string(url);

				if(!stream.parse(url_stream, url_data, false))
					throw std::runtime_error{"parse http client json message fail"};

				std::cout << url_data.toStyledString() << std::endl;

				init_signtx(cmd);
					
				/*
				if(json_init(cmd))
					msg = write_json_message(cmd);
				write_msg = parse_signtx_message(msg);
				while(!write_msg.empty()) {
					msg.clear();
					msg = write_json_message(write_msg);
					write_msg = parse_signtx_message(msg);
				}
				*/
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

		bool json_init(std::string const &cmd_json)
		{
			std::string path;
			path = "./json/" + cmd_json + ".json";
			input.open(path);
			assert(input.is_open());
			if(!reader.parse(input, json_msg, false)) {
				return false;	
			}

			return true;
		}

		Json::Value init_signtx(std::string const &path)
		{
			Json::Value msg, read_msg;
			msg["type"] = Json::Value("SignTx");
			msg["message"]["coin_name"] = json_msg[path]["coin_name"];
			msg["message"]["inputs_count"] = json_msg[path]["inputs_count"];
			msg["message"]["outputs_count"] = json_msg[path]["outputs_count"];
			read_msg = msg_cmd->message_communication(msg);
			
			return read_msg;
		}

		Json::Value pin_request()
		{
			Json::Value msg, pin_msg;
			std::string pin;
			std::cout << "Input Pin :";
			std::cin >> pin;	
			pin_msg["type"] = Json::Value("PinMatrixAck");
			pin_msg["message"]["pin"] = Json::Value(pin);
			//std::cout << "pin_msg : " << pin_msg.toStyledString() << std::endl;
			msg = msg_cmd->message_communication(pin_msg);

			return msg;
		}

		std::string parse_signtx_message(Json::Value const &msg)
		{
			Json::Value type_msg, details_msg;
			std::ostringstream req_index;
			std::string write_msg;

			type_msg = msg["message"];
			details_msg = type_msg["details"];
			req_index << details_msg["request_index"].asInt();

			if(!msg["type"].asString().compare("TxRequest")) {
				if(!type_msg["request_type"].asString().compare("TXINPUT")) {
					if(details_msg["tx_hash"].type() != Json::nullValue)
						write_msg = "tx_inputs" + req_index.str();
					else
						write_msg = "txinit_inputs" + req_index.str();
				}

				if(!type_msg["request_type"].asString().compare("TXOUTPUT")) {
					if(details_msg["tx_hash"].type() == Json::nullValue)
						write_msg = "tx_outputs" + req_index.str();
					else
						write_msg = "txbin_outputs" + req_index.str();
				}

				if(!type_msg["request_type"].asString().compare("TXMETA")) {
					write_msg = "tx_meta" + req_index.str();
				}
				std::cout << "write_msg : " << write_msg << std::endl;

				return write_msg;		
			}

			if(!msg["type"].asString().compare("ButtonRequest")) {
				write_msg = "button_ack";
				return write_msg;				
			}

			return nullptr;

		}

		Json::Value write_json_message(std::string const &msg)
		{
			Json::Value tx_msg, read_msg;
			tx_msg = json_msg[msg];
			std::cout << "tx_msg : " << tx_msg.toStyledString() << std::endl;
			read_msg = msg_cmd->message_communication(tx_msg);
			std::cout << "read_msg : " << read_msg.toStyledString() << std::endl;

			return read_msg;
		}
	};
}
