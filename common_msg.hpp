#include <jsoncpp/json/json.h>
#include <stdlib.h>
#include "http_client.hpp"

namespace common_msg {

	void parse_read_message(Json::Value &read_msg);
	Json::Reader reader;
	Json::Value url_data;
	Json::Value config_data;
	int total_amount;
	static int fee = 10000;

	Json::Value message_communication(Json::Value const &json)
	{   
		wire::message wire_in;
		wire::message wire_out; 
		std::unique_ptr<core::kernel> kernel(new core::kernel());
		auto dev = kernel->enumerate_devices();
		if(dev.empty())
			throw std::runtime_error{"no device found"};

		std::unique_ptr<core::device_kernel> device(new core::device_kernel(dev[0].first.path.c_str()));

		Json::Value json_msg;
		kernel->json_to_wire(json, wire_in);
		device->call(wire_in, wire_out);
		kernel->wire_to_json(wire_out, json_msg);

		return json_msg;
	}   

	void http_client_request_data(std::string & url)
	{
		auto url_stream = http_client::request_url_to_string(url);

		if(!reader.parse(url_stream, url_data, false))
			throw std::runtime_error{"parse http client json message fail"};

		std::cout << url_data.toStyledString() << std::endl;

	}	

	void write_signtx_init(Json::Value &json_msg)
	{   
		config_data = json_msg;
		Json::Value msg;
		msg["type"] = json_msg["type"];
		msg["message"]["coin_name"] = json_msg["coin_name"];
		msg["message"]["inputs_count"] = json_msg["inputs_count"];
		msg["message"]["outputs_count"] = json_msg["outputs_count"];
	//	std::cout << common_json.toStyledString()<< std::endl;
		auto read_msg = message_communication(msg);
		//std::cout << read_msg.toStyledString()<< std::endl;
		parse_read_message(read_msg);
	}   

	void write_txinit_inputs(int index, Json::Value &common_json)
	{  
		//Json::Value msg;
		common_json["message"]["tx"]["inputs"][index]["prev_hash"] = url_data["txid"];
		common_json["message"]["tx"]["inputs"][index]["prev_index"] = url_data["vin"][index]["n"];
		common_json["message"]["tx"]["inputs"][index]["address_n"] = config_data["address_n"];
		
		std::cout << common_json.toStyledString()<< std::endl;
		auto read_msg = message_communication(common_json);
		//std::cout << read_msg.toStyledString()<< std::endl;
		parse_read_message(read_msg);	
	}   

	void write_tx_meta(int index, Json::Value &common_json)
	{   
		common_json["message"]["tx"]["version"] = url_data["version"];
		common_json["message"]["tx"]["lock_time"] = url_data["locktime"];
		common_json["message"]["tx"]["inputs_cnt"] = Json::Value(url_data["vin"].size());
		common_json["message"]["tx"]["outputs_cnt"] = Json::Value(url_data["vout"].size());
		std::cout << common_json.toStyledString()<< std::endl;
		auto read_msg = message_communication(common_json);
		std::cout << read_msg.toStyledString()<< std::endl;
		parse_read_message(read_msg);	
	}   

	void write_tx_inputs(int index, Json::Value &common_json)
	{   
		common_json["message"]["tx"]["inputs"][0]["prev_hash"] = url_data["vin"][index]["txid"];
		common_json["message"]["tx"]["inputs"][0]["prev_index"] = url_data["vin"][index]["vout"];
		common_json["message"]["tx"]["inputs"][0]["sequence"] = url_data["vin"][index]["sequence"];
		common_json["message"]["tx"]["inputs"][0]["script_sig"] = url_data["vin"][index]["scriptSig"]["hex"];
		std::cout << common_json.toStyledString()<< std::endl;
		auto read_msg = message_communication(common_json);
		std::cout << read_msg.toStyledString()<< std::endl;
		parse_read_message(read_msg);	
	}

	void write_tx_outputs(int index, Json::Value &common_json)
	{
		common_json["message"]["tx"]["outputs"][0]["script_type"] = Json::Value("PAYTOADDRESS");

		if(index == 0) {
			common_json["message"]["tx"]["outputs"][0]["amount"] = total_amount - config_data["amount"].asInt() - fee;
			common_json["message"]["tx"]["outputs"][0]["address_n"] = config_data["address_n"];	
		}
		else {
			common_json["message"]["tx"]["outputs"][0]["amount"] = config_data["amount"];
			common_json["message"]["tx"]["outputs"][0]["address"] = config_data["payout_address"];	
		}
		std::cout << common_json.toStyledString()<< std::endl;
		auto read_msg = message_communication(common_json);
		std::cout << read_msg.toStyledString()<< std::endl;
		parse_read_message(read_msg);	
	}

	void write_txbin_outputs(int index, Json::Value &common_json)
	{
		auto value = url_data["vout"][index]["value"].asString();
		int amount  = static_cast<int>(atof(value.c_str()) * 100000000);
		//if(url_data["vout"][index]["spentTxId"].type() == Json::nullValue)
			total_amount = 200000;

		std::cout << "total_amount = " << total_amount << std::endl;
		
		common_json["message"]["tx"]["bin_outputs"][0]["amount"] = Json::Value(amount);
		common_json["message"]["tx"]["bin_outputs"][0]["script_pubkey"] = url_data["vout"][index]["scriptPubKey"]["hex"];
		std::cout << common_json.toStyledString()<< std::endl;
		auto read_msg = message_communication(common_json);
		std::cout << read_msg.toStyledString()<< std::endl;
		parse_read_message(read_msg);	
		
	}

	void write_button_ack()
	{
		Json::Value button_msg;
		button_msg["type"] = Json::Value("ButtonAck");
		auto read_msg = message_communication(button_msg);
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
		auto read_msg = message_communication(pin_msg);
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
		auto common_json = signtx_common_json();
		if(!read_msg["type"].asString().compare("TxRequest")) {
			req_index = type_msg["details"]["request_index"].asInt();

			if(!type_msg["request_type"].asString().compare("TXINPUT")) {
				if(details_msg["tx_hash"].type() != Json::nullValue) {
					write_tx_inputs(req_index, common_json);
				} else {
					write_txinit_inputs(req_index, common_json);
				}
			}

			if(!type_msg["request_type"].asString().compare("TXOUTPUT")) {
				if(details_msg["tx_hash"].type() == Json::nullValue) {
					write_tx_outputs(req_index, common_json);
				} else {
					write_txbin_outputs(req_index, common_json);
				}
			}

			if(!type_msg["request_type"].asString().compare("TXMETA")) {
				write_tx_meta(req_index, common_json);
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

}
