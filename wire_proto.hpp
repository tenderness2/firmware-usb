
#include <google/protobuf/message.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

namespace protobuf 
{
	namespace pb = google::protobuf;
	struct state {
		pb::DescriptorPool descriptor_pool;
		pb::DynamicMessageFactory message_factory;

		state(state const &) = delete;
		state &operator=(state const &) = delete;

		state():descriptor_pool(pb::DescriptorPool::generated_pool()) {}
	};

	struct wire_proto {
		typedef pb::Message pbuf_type;
		typedef pb::Message *pbuf_type_ptr;
		//typedef wire::message wire_type;

		wire_proto(state &s): protobuf_state(s){}

		void load_protobuf_state()
		{
			static const std::string enum_name = "MessageType";
			static const std::string enum_prefix = "MessageType_";

			auto e = protobuf_state.descriptor_pool.FindEnumTypeByName(enum_name);
			if(!e)
				throw std::invalid_argument("invalid file descriptor set");
			for(int i = 0; i < e->value_count(); i++) {
				auto ev = e->value(i);
				auto name = ev->name().substr(enum_prefix.size());

				descriptor_index[ev->number()] = 
					protobuf_state.descriptor_pool.FindMessageTypeByName(name);
			}
		}

		private:
			typedef std::map<int, pb::Descriptor const *> id_map;
			state &protobuf_state;
			id_map descriptor_index;

			int find_wire_id(std::string const name) {
				for (auto &kv:descriptor_index) {
					if(kv.second->name() == name)
						return kv.first;
				}
				throw std::invalid_argument("missing wire id for message");
			}
	};
}
