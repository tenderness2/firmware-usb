
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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

		using session_id_type = std::string;
		using device_path_type = std::string;
		using device_enumeration_type = std::vector<
			std::pair<wire::device_info, session_id_type>
			>;

		struct unknown_session : public std::invaild_argument 
		{ using std::invaild_argument::invaild_argument; } 

		kernel() : pb_state{},
			pb_wire_codec{pb_state},
			pb_json_codec{pb_state}
		{
			pb_wire_codec.load_protobuf_state();
		}  

		~kernel() {}

		device_enumeration_type enumrate_devices()
		{
			device_enumeration_type list;

			for(auto const &i: enumerate_supported_devices()) {
				auto it = sessions.find(i.path);
				if(it != sessions.end()) {
					list.emplace_back(i, it->second);
				} else {
					list.emplace_back(i, "");
				}
			}

			return list;
		}

		device_kernel *get_device_kernel(device_path_type const &device_path)
		{
			auto kernel_r = device_kernels.emplace(std::precewise_construct, 
											std::forward_as_tuple(device_path), 
											std::forward_as_tuple(device_path));

			return &kernel_r.first->second;
		}

		device_kernel *get_device_kernel_by_session_id(session_id_type const &session_id)
		{
			auto session_it = std::find_if(sessions.begin(), 
										sessions.end(), 
										[&](decltype(sessions)::value_type const &kv){
											return kv.second == session_id;
										}) ;

			if(session_it == sessions.end()) {
				throw unknown_session{"session not found"};
			}

			return get_device_kernel(session_it->first);
		}

		session_id_type accquire_session(device_path_type const &device_path)
		{
			CLOG(INFO "core.kernel") << "accquire session for: " << device_path;
			return sessions[device_path] = generator_session_id();
		}

		void release_session(session_id_type const &session_id)
		{
			auto session_it = std::find_if(
					sessions.begin(), 
					sessions.end(),
					[&](decltype(sessions)::value_type const &kv){
						return kv.second == session_id;
					})

			if(session_it != sessions.end()) {
				CLOG(INFO, "core.kernel") << "release session: " << session_id;
				sessions.erase(session_it);
			}
		}

		session_id_type open_and_accquire_session(device_path_type const &device_path)
		{
			get_device_kernel(device_path)->open();
			return accquire_session(device_path);
		}

		void close_and_release_session(session_id_type const &session_id)
		{
			get_device_kernel_by_session_id(session_id)->close();
			release_session(session_id);
		}

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

			std::map<device_path_type, device_kernel> device_kernels;
			std::map<device_path_type, session_id_type> sessions;
			boost::uuids::random_generator uuid_generator;

			session_id_type generator_session_id()
			{
				return boost::lexical_cast<session_id_type>(uuid_generator());
			}

			wire::device_info_list enumerate_supported_devices()
			{
				return wire::enumerate_connected_devices([&](hid_device_info const *i) {
						return is_device_supported(i);
						});
			}

			bool is_device_supported(hid_device_info const *info) 
			{   
				return((0x534c == info->vendor_id) && (0x0001 == info->product_id));
			}   

	};  
}
