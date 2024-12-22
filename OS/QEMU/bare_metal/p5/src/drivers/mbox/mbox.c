#include<mbox.h>
#include<printk.h>

int mbox_read(int channel){
	int data;
	do{
		do{asm volatile("nop");}while(MAILBOX_REGS->status & MBOX_EMPTY);	
		data = MAILBOX_REGS->read;
	}while((data & CHAN_MASK) != channel);
	return data;
}


int mbox_write(int channel, uint64_t data){
	unsigned int r = (((unsigned int)((unsigned long)data)&~0xF) | (channel&0xF));

	/* wait until we can write to the mailbox */
	do{asm volatile("nop");}while(MAILBOX_REGS->status & MBOX_FULL);

	MAILBOX_REGS->write = r;

	return 0;
}

int mbox_call(int channel, mbox_msg_t *msg){

	int data;
	unsigned int r = (((unsigned int)((unsigned long)msg)&~0xf) |  (channel & 0xf));
	mbox_write( channel, (uint64_t)msg);

	while(1){
		data = mbox_read(channel);
		if(data == r && msg->req_resp_code == MBOX_RESPONSE){
			return 0;
		}
	}
	return -1;
}
