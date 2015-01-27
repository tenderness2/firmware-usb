
#include <iostream>
#include "messages.hpp"
#include "messages.pb.h"
#include "traffic.hpp"

using namespace std;

TrafficData::TrafficData(){
	cout << "TrafficData init" << endl;
	//FirmwareUsb *firmware = new FirmwareUsb();
	//firmware->ConnectFirmware();
}

void TrafficData::ScreenTest(uint32_t time){
	TestScreen *resp;

	firmware.ConnectFirmware();
	resp = RespInit<TestScreen>();
	resp->delay_time = time;
	msg_write(MessageType_MessageType_TestScreen, resp);
	firmware.SendDataFirmware();
}

void TrafficData::BootInit() {
	firmware.ConnectFirmware();
	msg_write(MessageType_MessageType_Initialize, NULL);
	firmware.SendDataFirmware();
	msg_write(MessageType_MessageType_FirmwareErase, NULL);
	firmware.SendDataFirmware();
}

TrafficData::~TrafficData() {
	cout << "TrafficData exit " << endl;
}


