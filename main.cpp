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
#include <getopt.h>
#include <hidapi/hidapi.h>

#include "messages.pb.h"
#include "messages.hpp"
#include "firmware.hpp"
#include "traffic.hpp"

using namespace std;

static struct option opts[] = {
	{"help",    0, NULL, 'h'},
	{"print",   0, NULL, 'p'},
	{"version", 0, NULL, 'v'},
	{"time",    1, NULL, 't'},
	{0, 0, 0, 0}
};

enum mode {
	MODE_NONE,
	MODE_VERSION,
	MODE_PRINTDEV,
	MODE_SCREEN,
	MODE_FUNC,
};


static void PrintVersion(void)
{
	printf("verfiy 0.0.1\n\n");
	printf("Copyright 2015 kevin for Bidingxing.ltd\n"
			"This program is Bidingxing.ltd to verfiy bootloader and firmware data communication\n\n\n");

}

static inline void Help(void)
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
	TrafficData *data = new TrafficData();

	if(argc == 1)
		Help();

	while(1) {
		int c, option_index = 0;
		c = getopt_long(argc, argv, "fhpvt:", opts, &option_index);
		if(c == -1)
			break;

		switch(c) {
			case 'h' :
				Help();
				break;
			case 'p' :
				mode = MODE_PRINTDEV;
				PrintDev();
				break;
			case 't' :
				mode = MODE_SCREEN;
				data->ScreenTest(atoi(optarg));
				break;
			case 'v' :
				mode = MODE_VERSION;
				PrintVersion();
				break;
			case 'f' :
				mode = MODE_FUNC;
				data->BootInit();
				break;
			default :
				Help();
				break;
		}
	}

	if(mode == MODE_NONE)
		Help();

	delete data;
	return 0;
}
