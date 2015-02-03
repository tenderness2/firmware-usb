
#include <netinet/in.h>

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

		bool operator==(device_info const &rhs) const {
			return (vendor_id == rhs.vendor_id)
				&& (product_id == rhs.product_id)
				&& (serial_number == rhs.serial_number)
				&& (path == rhs.path);

		}

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

	struct device {
		typedef std::uint8_t char_type;
		typedef std::size_t size_type;

		struct open_error : public std::runtime_error {
			using std::runtime_error::runtime_error;
		};

		struct read_error : public std::runtime_error {
			using std::runtime_error::runtime_error;
		};

		struct write_error : public std::runtime_error {
			using std::runtime_error::runtime_error;
		};

		device(device const&) = delete;
		device &operator = (device const &) = delete;

		device(char const *path)
		{
			shared_hid_lock lock{hid_mutex};

			hid = hid_open_path(path);
			if(!hid) {
				throw open_error{"HID device open failed"};
			}
		}

		~device() {
			hid_close(hid);
		}

		void read_buffered(char_type *data, size_type len) {
			if(read_buffer.empty())
				buffer_report();
			size_type n = read_report_form_buffer(data, len);
			if(n < len)
				read_buffered(data + n, len - n);
				
		}

		void  write(char_type const *data, size_type len) {
			size_type n = write_report(data, len);
			if(n < len) 
				write(data + n, len - n);
		}

		private :
		size_type read_report_form_buffer(char_type *data, size_type len) {
			using namespace std;

			size_type n = min(read_buffer.size(), len);
			auto r1 = read_buffer.begin();
			auto r2 = read_buffer.begin() + n ;
			copy(r1, r2, data);
			read_buffer.erase(r1, r2);

			return n;
		}

		void buffer_report() {
			using namespace std;

			report_type report;

			int r = [&] {
				shared_hid_lock lock{hid_mutex};
				return hid_read(hid, report.data(), report.size());
			}();

			if(r < 0)
				throw read_error {"HID device read failed"};

			if(r > 0) {
				char_type rn = report[0];
				size_type n = min(static_cast <size_type>(rn), static_cast<size_type>(r - 1));

				copy(report.begin() + 1, report.begin() + 1 + n, back_inserter(read_buffer));
			}
		}

		size_type write_report(char_type const *data, size_type len) {
			using namespace std;

			report_type report;

			report.fill(0x00);
			report[0] = report.size() - 1;

			size_type n = min(report.size() - 1, len);
			copy(data, data + n, report.begin() + 1);

			int r = [&] {
				shared_hid_lock lock{hid_mutex};
				return hid_write(hid, report.data(), report.size());
			}();

			if(r < 0)
				throw write_error{"HID device write failed"};

			if((unsigned int )r < report.size())
				throw write_error{"HID device write was insufficient"};

			return n;
		}
		
		typedef std::vector<char_type> buffer_type;
		typedef std::array<char_type, 64> report_type;

		hid_device *hid;
		buffer_type read_buffer;
	};

	struct message {
		std::uint16_t id;
		std::vector<std::uint8_t> data;

		typedef device::read_error header_read_error;

		void read_from(device &device) {
			device::char_type buf[6];
			std::uint32_t size;

			device.read_buffered(buf, 1);
			while(buf[0] != '#') {
				device.read_buffered(buf, 1);
			}

			device.read_buffered(buf, 1);
			if(buf[0] != '#') {
				throw header_read_error{"header bytes ar malformed"};
			}

			device.read_buffered(buf, 6);

			id = ntohs((buf[0] << 0) | (buf[1] << 8));
			size = ntohs((buf[2] << 0) | (buf[3] << 8) | (buf[4] << 16) | (buf[5] << 24));

			static const std::uint32_t max_size = 1024 * 1024;
			if(size > max_size) {
				throw header_read_error("message is too big");
			}

			data.resize(size);
			device.read_buffered(data.data(), data.size());
		}	

		void write_to(device &device) const {
			std::size_t buf_size = 8 + data.size();
			device::char_type buf[buf_size];

			buf[0] = '#';
			buf[1] = '#';

			std::uint16_t id = htons(id);
			buf[2] = (id >> 0) & 0xff;
			buf[3] = (id >> 8) & 0xff;

			std::uint32_t size = htons(data.size());
			buf[4] = (size >> 0) & 0xff;
			buf[5] = (size >> 8) & 0xff;
			buf[6] = (size >> 16) & 0xff;
			buf[7] = (size >> 24) & 0xff;

			copy(data.begin(), data.end(), &buf[8]);
			device.write(buf, buf_size);
		}
	};
}
