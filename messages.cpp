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
 **********************************************************/

#include <iostream>

#include "messages.hpp"
#include "pb_decode.h"
#include "pb_encode.h"
#include "messages.pb.h"

struct MessagesMap_t {
	char type;
	char dir;
	uint16_t msg_id;
	const pb_field_t *fields;
	void (*process_func)(void *ptr);
};

static const struct MessagesMap_t MessagesMap[] = {
	//out messages
	{'n', 'o', MessageType_MessageType_TestScreen,		TestScreen_fields, 0},

	//end
	{0, 0, 0, 0, 0}
};

const pb_field_t *MessageFields(char type, char dir, uint16_t msg_id)
{
	const struct MessagesMap_t *m = MessagesMap;
	while (m->type) {
		if (type == m->type && dir == m->dir && msg_id == m->msg_id) {
			return m->fields;

		}
		m++;

	}
	return 0;

}

static uint32_t msg_out_start = 0;
static uint32_t msg_out_end = 0;
static uint32_t msg_out_cur = 0;
static uint8_t msg_out[MSG_OUT_SIZE];

inline void msg_out_append(uint8_t c)
{
	if (msg_out_cur == 0) {
		msg_out[msg_out_end * 64] = '?';
		msg_out_cur = 1;

	}
	msg_out[msg_out_end * 64 + msg_out_cur] = c;
	msg_out_cur++;
	if (msg_out_cur == 64) {
		msg_out_cur = 0;
		msg_out_end = (msg_out_end + 1) % (MSG_OUT_SIZE / 64);

	}

}

inline void msg_out_pad(void)
{
	if (msg_out_cur == 0) return;
	while (msg_out_cur < 64) {
		msg_out[msg_out_end * 64 + msg_out_cur] = 0;
		msg_out_cur++;

	}
	msg_out_cur = 0;
	msg_out_end = (msg_out_end + 1) % (MSG_OUT_SIZE / 64);

}

uint8_t *msg_out_data(void)
{
	if (msg_out_start == msg_out_end) return 0;
	uint8_t *data = msg_out + (msg_out_start * 64);
	msg_out_start = (msg_out_start + 1) % (MSG_OUT_SIZE / 64);
	return data;

}

static bool pb_callback_out(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
	(void)stream;
	size_t i;
	for (i = 0; i < count; i++) {
		msg_out_append(buf[i]);

	}
	return true;

}

bool msg_write_common(char type, uint16_t msg_id, const void *msg_ptr)
{
	const pb_field_t *fields = MessageFields(type, 'o', msg_id);
	if (!fields) { // unknown message
		return false;
	}

	pb_ostream_t sizestream = {0, 0, SIZE_MAX, 0, 0};
	bool status = pb_encode(&sizestream, fields, msg_ptr);

	if (!status) {
		return false;

	}

	void (*append)(uint8_t);
	bool (*pb_callback)(pb_ostream_t *, const uint8_t *, size_t);

	if (type == 'n') {
		append = msg_out_append;
		pb_callback = pb_callback_out;

	} else { 
		return false;
	}

	uint32_t len = sizestream.bytes_written;
	append('#');
	append('#');
	append((msg_id >> 8) & 0xFF);
	append(msg_id & 0xFF);
	append((len >> 24) & 0xFF);
	append((len >> 16) & 0xFF);
	append((len >> 8) & 0xFF);
	append(len & 0xFF);
	pb_ostream_t stream = {pb_callback, 0, SIZE_MAX, 0, 0};
	status = pb_encode(&stream, fields, msg_ptr);
	if (type == 'n') {
		msg_out_pad();
	}

	return status;
}
