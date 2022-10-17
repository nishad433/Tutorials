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
	uint32_t read;
	uint32_t reserved1[5];
#define MBOX_EMPTY (1u << 30)
#define MBOX_FULL (1u << 31)
	uint32_t status;
	uint32_t reserved2;
	uint32_t write;
	
}mbox_regs_t;

typedef struct {
	uint32_t buff_size;
	uint32_t req_resp_code;
	uint32_t tag_id;
	uint32_t tag_buff_size;
	uint32_t tag_req_resp_code;
	uint32_t tag_buff[31];
}__attribute__((aligned(16),packed)) mbox_msg_t;

#define MAILBOX_BASE	(IO_BASE_ADDR + 0xB880)
#define MAILBOX_REGS ((volatile mbox_regs_t *)MAILBOX_BASE)

int mbox_call(int channel, mbox_msg_t *msg);

#endif //MBOX_SUPPORT

#endif //_MBOX_H_
