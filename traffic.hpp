#ifndef __TRAFFIC_H__
#define __TRAFFIC_H__

#include "messages.hpp"
#include "firmware.hpp"

static uint8_t msg_resp[MSG_OUT_SIZE];
#define RESP_INIT(TYPE) TYPE *resp = (TYPE *)msg_resp; memset(resp, 0, sizeof(TYPE));

class TrafficData {
	public :
		TrafficData();
		void ScreenTest(uint32_t time);
		~TrafficData();

	private :
		FirmwareUsb *firmware;
};

#endif //__TRAFFIC_H__
