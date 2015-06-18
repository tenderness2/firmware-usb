
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

	struct interactive_message {

		Json::Value message_communication(Json::Value const &json)
		{
			wire::message wire_in;
			wire::message wire_out;	
			std::unique_ptr<core::kernel> kernel(new core::kernel());
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
				msg_cmd.reset(new interactive_message);
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

				auto read_msg = msg_cmd->message_communication(json_msg[cmd]);
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		void get_features(std::string const &cmd) 
		{
			try {
				if(!json_init(cmd))
					throw std::runtime_error{"parse json message fail"};
				auto read_msg = msg_cmd->message_communication(json_msg[cmd]);
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

				auto read_msg = msg_cmd->message_communication(json_msg[cmd]);
				parse_read_message(read_msg);
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

				auto url = json_msg["url"].asString() + json_msg["prev_hash"].asString();	
				auto url_stream = http_client::request_url_to_string(url);

				if(!reader.parse(url_stream, url_data, false))
					throw std::runtime_error{"parse http client json message fail"};

				std::cout << url_data.toStyledString() << std::endl;

				write_signtx_init();

					
			} catch(std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		~device_command() 
		{
			msg_cmd.reset();
		}

		private :
			std::unique_ptr<interactive_message> msg_cmd;
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

		void write_signtx_init()
		{
			Json::Value msg;
			msg["type"] = json_msg["type"];
			msg["message"]["coin_name"] = json_msg["coin_name"];
			msg["message"]["inputs_count"] = json_msg["inputs_count"];
			msg["message"]["outputs_count"] = json_msg["outputs_count"];
			auto read_msg = msg_cmd->message_communication(msg);
			parse_read_message(read_msg);
		}

		void write_txinit_inputs(int index, Json::Value &common_msg)
		{
			Json::Value msg;
			
		}

		void write_tx_meta(int index, Json::Value &common_msg)
		{

		}

		void write_tx_inputs(int index, Json::Value &common_msg)
		{
			
		}

		void write_tx_outputs(int index, Json::Value &common_msg)
		{

		}

		void write_txbin_outputs(int index, Json::Value &common_msg)
		{
			
		}

		void write_button_ack()
		{
			Json::Value button_msg;
			button_msg["type"] = Json::Value("ButtonAck");
			auto read_msg = msg_cmd->message_communication(button_msg);
			parse_read_message(read_msg);
		}

		void write_pin_ack()
		{
			Json::Value pin_msg;
			std::string pin;
			std::cout << "Please Input Pin :";
			std::cin >> pin;	
			pin_msg["type"] = Json::Value("PinMatrixAck");
			pin_msg["message"]["pin"] = Json::Value(pin);
			auto read_msg = msg_cmd->message_communication(pin_msg);
			//std::cout << "read_msg : " << read_msg.toStyledString() << std::endl;
			parse_read_message(read_msg);
		}

		Json::Value signtx_common_json()
		{
			Json::Value msg;
			msg["type"] = Json::Value("TxAck");
			msg["message"]["has_tx"] = Json::Value(true);

			return msg;
		}

		void parse_read_message(Json::Value &read_msg)
		{
			int req_index;

			auto type_msg = read_msg["message"];
			auto details_msg = type_msg["details"];
			auto common_msg = signtx_common_json();
			if(!read_msg["type"].asString().compare("TxRequest")) {
				req_index = type_msg["details"]["request_index"].asInt();

				if(!type_msg["request_type"].asString().compare("TXINPUT")) {
					if(details_msg["tx_hash"].type() != Json::nullValue) {
						write_tx_inputs(req_index, common_msg);
					} else {
						write_txinit_inputs(req_index, common_msg);
					}
				}

				if(!type_msg["request_type"].asString().compare("TXOUTPUT")) {
					if(details_msg["tx_hash"].type() == Json::nullValue) {
						write_tx_outputs(req_index, common_msg);
					} else {
						write_txbin_outputs(req_index, common_msg);
					}
				}

				if(!type_msg["request_type"].asString().compare("TXMETA")) {
					write_tx_meta(req_index, common_msg);
				}
			}

			if(!read_msg["type"].asString().compare("ButtonRequest")) {
				write_button_ack();
			}

			if(!read_msg["type"].asString().compare("PinMatrixRequest")) {
				write_pin_ack();
			}

			if(read_msg["type"].type() == Json::nullValue) {
					throw std::runtime_error{"unkown json message type"};	
			}

		}

	};
}
