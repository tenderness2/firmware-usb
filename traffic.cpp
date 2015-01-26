
#include <iostream>
#include "messages.hpp"
#include "messages.pb.h"
#include "traffic.hpp"

using namespace std;

TrafficData::TrafficData(){
	cout << "TrafficData init" << endl;
}

void TrafficData::ScreenTest(uint32_t time){
	cout << "this is TrafficData test" << endl;
	FirmwareUsb *firmware = new FirmwareUsb();
	firmware->ConnectFirmware();
	RESP_INIT(TestScreen);
	resp->delay_time = time;
	msg_write(MessageType_MessageType_TestScreen, resp);
	firmware->SendDataFirmware();
	delete firmware;
}

TrafficData::~TrafficData() {
	cout << "TrafficData exit " << endl;
}


