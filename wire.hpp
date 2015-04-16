
#include <string>
#include <cstdint>
#include <vector>
#include <array>

namespace wire {

	struct device_info{

		std::uint16_t vendor_id;
		std::uint16_t product_id;
		std::wstring serial_number;
		std::string path;

		bool operator==(device_info const &rhs) const 
		{
			return (vendor_id == rhs.vendor_id) 
				&& (product_id == rhs.product_id) 
				&& (serial_number == rhs.serial_number)
				&& (path == rhs.path);
		}
	};

	typedef std::vector<device_info> device_info_list;

	bool is_device_supported(hid_device_info const *info) 
	{
		return((0x534c == info->vendor_id) && (0x0001 == info->product_id));
	}

	device_info_list enumerate_connected_devices()
	{
		device_info_list list;
		auto *infos = hid::enumerate(0x00, 0x00);

		for(auto i = infos; i != nullptr; i = i->next) {
			if(!is_device_supported(i)) 
				continue;

			if(i->interface_number > 0){
				CLOG(DEBUG, "wire.enumerate") << "skipping, invaild device";
				continue;
			}
			list.emplace_back(device_info{i->vendor_id, i->product_id, 
											i->serial_number, i->path});

			hid::free_enumeration(infos);

		}
		return list;
	}

	struct device {
		
		device(device const &) = delete;
		device &operator=(device const &) = delete;

		struct open_error :public std::runtime_error
		{ using std::runtime_error::runtime_error; };

		struct read_error :public std::runtime_error
		{ using std::runtime_error::runtime_error; };

		struct write_error :public std::runtime_error
		{ using std::runtime_error::runtime_error; };

		device(char const *path) 
		{
			hid = hid::open_path(path);
			if(!hid) {
				throw open_error("HID device open fail !");
			}
		}

		~device() {hid::close(hid);}

		private :
			hid_device *hid;
	};

	struct device_kernel {
		using device_path_type = std::string;
		device_path_type device_path;

		device_kernel(device_path_type const &dp):device_path{dp}
		{ }

		void open()
		{
			if(device.get() == nullptr) {
				CLOG(INFO, "wire.device") << "opening : " << device_path;
				device.reset(new wire::device{device_path.c_str()});
			}
		}

		void close()
		{
			CLOG(INFO, "wire.device") << "closing : " << device_path;
			device.reset();
		}

		private :
			std::unique_ptr<wire::device> device;

	};

}
