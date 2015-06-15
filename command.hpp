
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
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};

				write_config_json(cmd);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void get_features(std::string const &cmd) 
		{
			try {
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};
				write_config_json(cmd);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void set_label(std::string const &cmd)
		{
			try {
				if(!json_init(cmd)) 
					throw std::runtime_error{"parse json message fail"};

				write_config_json(cmd);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void sign_tx(std::string const &cmd)
		{
			try {
				Json::Value url_data;

				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};

				auto url = json_msg[cmd]["url"].asString() + json_msg[cmd]["prev_hash"].asString();	
				auto url_stream = http_client::request_url_to_string(url);

				if(!reader.parse(url_stream, url_data, false))
					throw std::runtime_error{"parse http client json message fail"};

				write_signtx_init();

				//std::cout << url_data.toStyledString() << std::endl;
					
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

		void write_signtx_init()
		{
			Json::Value msg;
			msg["type"] = json_msg["type"];
			msg["message"]["coin_name"] = json_msg["coin_name"];
			msg["message"]["inputs_count"] = json_msg["inputs_count"];
			msg["message"]["outputs_count"] = json_msg["outputs_count"];
			write_type_json(msg);
		}

		void write_txinit_inputs(int index)
		{
			
		}

		void write_tx_meta(int index)
		{

		}

		void write_tx_inputs(int index)
		{
			
		}

		void write_tx_outputs(int index)
		{

		}

		void write_txbin_outputs(int index)
		{
			
		}

		void write_button_ack()
		{
			Json::Value button_msg;
			button_msg["type"] = Json::Value("ButtonAck");
			write_type_json(button_msg);
		}

		void write_pin_ack()
		{
			Json::Value pin_msg;
			std::string pin;
			std::cout << "Please Input Pin :";
			std::cin >> pin;	
			pin_msg["type"] = Json::Value("PinMatrixAck");
			pin_msg["message"]["pin"] = Json::Value(pin);
			write_type_json(pin_msg);
			//std::cout << "pin_msg : " << pin_msg.toStyledString() << std::endl;
		}

		void parse_read_message(Json::Value const &msg)
		{
			int req_index;

			auto type_msg = msg["message"];
			auto details_msg = type_msg["details"];
			if(!msg["type"].asString().compare("TxRequest")) {
				req_index = type_msg["details"]["request_index"].asInt();

				if(!type_msg["request_type"].asString().compare("TXINPUT")) {
					if(details_msg["tx_hash"].type() != Json::nullValue) {
						write_tx_inputs(req_index);
					} else {
						write_txinit_inputs(req_index);
					}
				}

				if(!type_msg["request_type"].asString().compare("TXOUTPUT")) {
					if(details_msg["tx_hash"].type() == Json::nullValue) {
						write_tx_outputs(req_index);
					} else {
						write_txbin_outputs(req_index);
					}
				}

				if(!type_msg["request_type"].asString().compare("TXMETA")) {
					write_tx_meta(req_index);
				}
			}

			if(!msg["type"].asString().compare("ButtonRequest")) {
				write_button_ack();
			}

			if(!msg["type"].asString().compare("PinMatrixRequest")) {
				write_pin_ack();
			}

			if(msg["type"].type() == Json::nullValue) {
					throw std::runtime_error{"unkown json message type"};	
			}

		}

		void write_type_json(Json::Value const &msg)
		{
			Json::Value read_msg;
			read_msg = msg_cmd->message_communication(msg);
			std::cout << "read_msg : " << read_msg.toStyledString() << std::endl;
			parse_read_message(read_msg);	
		}

		void write_config_json(std::string const &msg)
		{
			Json::Value tx_msg, read_msg;
			tx_msg = json_msg[msg];
			std::cout << "tx_msg : " << tx_msg.toStyledString() << std::endl;
			read_msg = msg_cmd->message_communication(tx_msg);
			std::cout << "read_msg : " << read_msg.toStyledString() << std::endl;
			parse_read_message(read_msg);
		}
	};
}
