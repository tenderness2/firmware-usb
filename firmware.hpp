
#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

#include <hidapi/hidapi.h>

class FirmwareUsb {
	public :
		FirmwareUsb();
		void ConnectFirmware();
		void PrintUsbDev();
		void SendDataFirmware() const;
		~FirmwareUsb();

	private:
		struct hid_device_info *devs, *cur_dev;
		hid_device *handle;
		int product_id ;
		int vendor_id ;
};


#endif //__FIRMWARE_H__
