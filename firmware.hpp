
#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

#include <getopt.h>

static uint8_t msg_resp[MSG_OUT_SIZE];
#define RESP_INIT(TYPE) TYPE *resp = (TYPE *)msg_resp; memset(resp, 0, sizeof(TYPE));

static struct option opts[] = {
	{"time", 1, NULL, 't'},
	{0, 0, 0, 0}
};


class FirmwareUsb {
	public :
		FirmwareUsb();
		void ConnectFirmware();
		void PrintUsbDev();
		void SendDataFirmware() const;
		void RespInit(FirmwareUsb const &data, uint32_t time) const;
		~FirmwareUsb();

	private:
		struct hid_device_info *devs, *cur_dev;
		hid_device *handle;
		int product_id ;
		int vendor_id ;
};


#endif //__FIRMWARE_H__
