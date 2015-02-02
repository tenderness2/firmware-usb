
#include <hidapi/hidapi.h>
#include <boost/thread.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <array>

namespace wire {

	static boost::shared_mutex hid_mutex;

	using shared_hid_lock = boost::shared_lock<boost::shared_mutex>;
	using unique_hid_lock = boost::unique_lock<boost::shared_mutex>;

	struct device_info {
		std::uint16_t vendor_id;
		std::uint16_t product_id;
		std::wstring serial_number;
		std::string path;
	};

	typedef std::vector<device_info> device_info_list;

	device_info_list enumerate_connected_devices(const boost::optional<int> & vendor_id, const boost::optional <int> & product_id) {
		unique_hid_lock lock{hid_mutex};
		device_info_list list;
		uint16_t vendorId, productId;
		vendorId = (vendor_id ? (uint16_t )*vendor_id : 0x00);
		productId = (product_id ? (uint16_t)*product_id : 0x00);
		auto *infos = hid_enumerate(vendorId, productId);

		for(auto i = infos; i != nullptr; i = i->next) {
			if((i->interface_number >0) ||
					(i->product_string == nullptr)) {
				CLOG(DEBUG, "wire.enumerate") << "skipping, invalid device";
				continue;
			}
			//printf("serial_number : %ls\n", i->serial_number);
			list.emplace_back(
					device_info{
					i->vendor_id,
					i->product_id,
					i->serial_number,
					i->path
					}
					);
		}
		hid_free_enumeration(infos);

		return list;
	}
}
