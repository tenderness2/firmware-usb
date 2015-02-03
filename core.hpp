#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

namespace core {
	struct device_kernel {
		using device_path_type = std::string;

		device_path_type device_path;
		device_kernel(device_path_type const&dp):device_path{dp} { }

		void open() {
			if(device.get() == nullptr) {
				CLOG(INFO, "core.device") << "opening : " << device_path;
				device.reset(new wire::device{device_path.c_str()});
			}	
		}

		void close() {
			CLOG(INFO,"core.device") << "closing : " << device_path;
			device.reset();
		}

		void call(wire::message const &msg_in, wire::message &msg_out) {
			CLOG(INFO, "core.device") << "calling : " << device_path;
			if(!device.get())
				open();

			try {
				msg_in.write_to(*device);
				msg_out.read_from(*device);
			} catch(std::exception const &e) {
				CLOG(ERROR, "core.device") << e.what();
				close();
				throw;
			}
		}

		private :
			std::unique_ptr<wire::device> device;
	};
}
