/*******************************************************
 *  BWallet usb communication
 *
 *  kevin 
 *  Bidingxing Software
 *     
 *  01/12/2015
 *
 *  Copyright 2015
 *        
 *********************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <sysexits.h>
#include <hidapi/hidapi.h>

#include "messages.pb.h"
#include "messages.hpp"
#include "firmware.hpp"

using namespace std;

FirmwareUsb::FirmwareUsb(): product_id(0x534c), vendor_id(0x0001) {
	cout << "FirmwareUsb init" << endl;
	if(hid_init())
		throw exception();
}

/*
FirmwareUsb::FirmwareUsb(TrafficData &data) {
	TrafficData *data = new TrafficData(); 
}
*/
void FirmwareUsb::ConnectFirmware() {
	int res;
	const int MAX_STR = 255;
	wchar_t usb_str[MAX_STR];
	handle = hid_open(product_id, vendor_id, NULL);
	if(!handle) {
		cout << "Unable to open device" << endl;
		throw exception();
	}

	usb_str[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, usb_str, MAX_STR);
	if(res < 0)
		cout << "Unable to read Manufacturer string" << endl;
//	cout << "Manufacturer string : " << usb_str << endl;
	printf("Manufacturer String: %ls\n", usb_str);

	usb_str[0] = 0x0000;
	res = hid_get_product_string(handle, usb_str, MAX_STR);
	if(res < 0)
		cout << "Unable to read Product string" << endl;
//	cout << "Product string : " << usb_str << endl;
	printf("Product String: %ls\n", usb_str);

	usb_str[0] = 0x0000;
	res = hid_get_serial_number_string(handle, usb_str, MAX_STR);
	if(res < 0)
		cout << "Unable to read serial number string" << endl;
//	cout << "serial number string : " << usb_str << endl;
 	printf("Serial Number String: (%d) %ls\n", usb_str[0], usb_str);

	usb_str[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, usb_str, MAX_STR);
	if(res < 0)
		cout << "Unable to read indexed string" << endl;
//	cout << "indexed string : " << usb_str << endl;
	printf("Indexed String 1: %ls\n", usb_str);
}

void FirmwareUsb::SendDataFirmware() const {

	int res;
	static uint8_t *data;

	data = msg_out_data();
	hid_set_nonblocking(handle, 1);
	res = hid_write(handle, data, 64);

	if(res < 0){
		cout << "Unable to write ";
		printf("%ls \n", hid_error(handle));
	}
	hid_close(handle);
}


void FirmwareUsb::RespInit(FirmwareUsb const *data, uint32_t time) const{
	RESP_INIT(TestScreen);
	resp->delay_time = time;
	msg_write(MessageType_MessageType_TestScreen, resp);
	data->SendDataFirmware();
}

void FirmwareUsb::PrintUsbDev() {
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	while(cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
}

FirmwareUsb::~FirmwareUsb() {
	cout << "FirmwareUsb exit" <<endl;
	hid_exit();
}

static void print_version(void)
{
	printf("verfiy 0.0.1\n\n");
	printf("Copyright 2015 kevin for Bidingxing.ltd\n"
			"This program is Bidingxing.ltd to verfiy bootloader and firmware data communication\n\n\n");

}


static void ScreeTest(uint32_t time)
{
	FirmwareUsb *firmware = new FirmwareUsb();
	firmware->ConnectFirmware();
	firmware->RespInit(firmware, time);
	delete firmware;
}

static void test()
{
	FirmwareUsb *firmware = new FirmwareUsb();
	firmware->Data.ScreenTest(5);
	delete firmware;
}

static inline void help(void)
{
	fprintf(stderr, "Usage: dfu-util [options] ...\n"
			"  -h --help\t\t\tPrint this help message\n"
			"  -v --version\t\t\tPrint the version number\n"
			"  -p --print\t\t\tList currently usb devices\n");
	fprintf(stderr, "  -t --time\t\t\tTest BWallet Screen time \n");
	exit(EX_USAGE);

}

static inline void PrintDev(void)
{
	FirmwareUsb *firmware = new FirmwareUsb();
	firmware->PrintUsbDev();
	delete firmware;
}

int main(int argc, char *argv[])
{
	enum mode mode = MODE_NONE;
	setvbuf(stdout, NULL, _IONBF, 0);

	if(argc == 1)
		help();

	while(1) {
		int c, option_index = 0;
		c = getopt_long(argc, argv, "fhpvt:", opts, &option_index);
		if(c == -1)
			break;

		switch(c) {
			case 'h' :
				help();
				break;
			case 'p' :
				mode = MODE_PRINTDEV;
				PrintDev();
				break;
			case 't' :
				mode = MODE_SCREEN;
				ScreeTest(atoi(optarg));
				break;
			case 'v' :
				mode = MODE_VERSION;
				print_version();
				break;
			case 'f' :
				mode = MODE_FUNC;
				test();
				break;
			default :
				help();
				break;
		}
	}

	if(mode == MODE_NONE)
		help();

	return 0;
}
