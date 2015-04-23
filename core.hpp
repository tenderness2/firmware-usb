
namespace core {

    struct device_kernel {
        using device_path_type = std::string;
        device_path_type device_path;

        device_kernel(device_path_type const &dp):device_path{dp}
        { } 

        void open()
        {   
        	if(device.get() == nullptr) {
                CLOG(INFO, "core.device") << "opening : " << device_path;
                device.reset(new wire::device{device_path.c_str()});
            }   
        }   

        void close()
        {   
            CLOG(INFO, "core.device") << "closing : " << device_path;
            device.reset();
        }   

        void call(wire::message const &msg_in, wire::message &msg_out)
        {
            CLOG(INFO, "core.device") << "calling: " << device_path;
            if(!device.get()){
                open();
            }
            try {
                msg_in.write_to(*device);
                msg_out.read_from(*device);
            }
            catch (std::exception const &e) {
                CLOG(ERROR, "core.device") << e.what();
                close();
                throw;
            }
        }

        private :
            std::unique_ptr<wire::device> device;
    };

	struct kernel {

		kernel() : pb_state{},
			pb_wire_codec{pb_state},
			pb_json_codec{pb_state}
		{
			pb_wire_codec.load_protobuf_state();
		}  

		~kernel() {}

		void json_to_wire(Json::Value const &json, wire::message &wire)
		{   
			protobuf_ptr pbuf{pb_json_codec.typed_json_to_protobuf(json)};
			pb_wire_codec.protobuf_to_wire(*pbuf, wire);
		}   

		void wire_to_json(wire::message const &wire, Json::Value &json)
		{   
			protobuf_ptr pbuf{pb_wire_codec.wire_to_protobuf(wire)};
			json = pb_json_codec.protobuf_to_typed_json(*pbuf);
		}   

		private:
			using protobuf_ptr = std::unique_ptr<protobuf::pb::Message>;
			protobuf::state pb_state;
			protobuf::wire_proto pb_wire_codec;
			protobuf::json_codec pb_json_codec;    
	};  
}
