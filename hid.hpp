
#include <hidapi/hidapi.h>

namespace hid{

	void init() { hid_init(); }
	void exit() { hid_exit(); }

	hid_device_info *enumerate(unsigned short vendor_id, unsigned short product_id)
	{
		return hid_enumerate(vendor_id, product_id);
	}

	void free_enumeration(hid_device_info *devs)
	{
		hid_free_enumeration(devs);
	}

	hid_device *open_path(char const *path)
	{
		return hid_open_path(path);
	}

	void close(hid_device *devs)
	{
		hid_close(devs);
	}

	int write(hid_device *device, unsigned char const *data, size_t length)
	{
		return hid_write(device, data, length);
	}

	int read(hid_device *device, unsigned char *data, size_t length)
	{
		return hid_read(device, data, length);
	}

	int read_timeout(hid_device *device, unsigned char *data, size_t length, int milliseconds)
	{
		return hid_read_timeout(device, data, length, milliseconds);
	}

	int send_feature_report(hid_device *device, unsigned char const *data, size_t length)
	{
		return hid_send_feature_report(device, data, length);
	}
}
