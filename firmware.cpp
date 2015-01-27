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

	int res, i;
	static uint8_t *data;
	cout << "SendDataFirmware test" << endl;
	data = msg_out_data();
	cout << "data: " << endl;
	for(i = 0; i < 64; i++)
		printf("%x ", data[i]);
	printf("\n");
	cout << "SendDataFirmware test 1" << endl;
	hid_set_nonblocking(handle, 1);
	cout << "SendDataFirmware test 2" << endl;
	res = hid_write(handle, data, 64);

	cout << "SendDataFirmware test 3" << endl;
	if(res < 0){
		cout << "Unable to write ";
		printf("%ls \n", hid_error(handle));
	}
	//hid_close(handle);
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

