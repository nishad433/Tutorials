#ifndef _MBOX_H_
#define _MBOX_H_

#include<platform.h>
#include<stdint.h>

#if defined(MAILBOX_SUPPORT)

#define CHAN_MASK	((1u<<4)-1)
#define DATA_OFFSET	(1u<<4)

#define MBOX_REQUEST    0
#define MBOX_RESPONSE	0x80000000

typedef enum _mbox_channel{
/* channels */
	MBOX_CH_POWER  = 0,
	MBOX_CH_FB     = 1,
	MBOX_CH_VUART  = 2,
	MBOX_CH_VCHIQ  = 3,
	MBOX_CH_LEDS   = 4,
	MBOX_CH_BTNS   = 5,
	MBOX_CH_TOUCH  = 6,
	MBOX_CH_COUNT  = 7,
	MBOX_CH_PROP   = 8
} mbox_channel;

typedef enum _mbox_tags {
	/* tags */
	MBOX_TAG_GETSERIAL	= 0x10004,
	MBOX_TAG_SETCLKRATE     = 0x38002,
	MBOX_TAG_GETCLKRATE     = 0x30002,
	MBOX_TAG_LAST           = 0,
}mbox_tags;

typedef struct {
	rw_REG32 read;
	ro_REG32 reserved1[5];
#define MBOX_EMPTY (1u << 30)
#define MBOX_FULL (1u << 31)
	rw_REG32 status;
	ro_REG32 reserved2;
	rw_REG32 write;
	
}mbox_regs_t;

typedef struct {
	rw_REG32 buff_size;
	rw_REG32 req_resp_code;
	rw_REG32 tag_id;
	rw_REG32 tag_buff_size;
	rw_REG32 tag_req_resp_code;
	rw_REG32 tag_buff[31];
}__attribute__((aligned(16),packed)) mbox_msg_t;

#define MAILBOX_REGS ((volatile mbox_regs_t *)MAILBOX_BASE)

int mbox_call(int channel, mbox_msg_t *msg);

#endif //MBOX_SUPPORT

#endif //_MBOX_H_
