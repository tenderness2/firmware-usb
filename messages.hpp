#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <stdint.h>
#include <stdbool.h>

#define MSG_OUT_SIZE (9*1024)

//#define msg_read(buf, len) msg_read_common('n', (buf), (len))
#define msg_write(id, ptr) msg_write_common('n', (id), (ptr))
uint8_t *msg_out_data(void);

bool msg_write_common(char type, uint16_t msg_id, const void *msg_ptr);

extern uint8_t msg_tiny[64];

#endif //__MESSAGES_H__

