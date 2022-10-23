#include<platform.h>
#include<gpio.h>
#include<uart.h>
#include<printk.h>
#include<mbox.h>
#include<rng.h>

int main(void){
#if defined(UART0_SUPPORT) || defined(UART1_SUPPORT)
        uart_cfg_t uart_cfg={
                .data_bits = 8,
                .stop_bits = 1,
                .parity = 0,
                .baudrate = 115200
        };
#endif

#if defined(GPIO_SUPPORT)	
	gpio_init();
#endif
#if defined(UART0_SUPPORT) || defined(UART1_SUPPORT)
	uart_configure(uart_cfg);
#endif

#if defined(MAILBOX_SUPPORT)
	mbox_msg_t msg;
	msg.buff_size = 8*4;
	msg.req_resp_code = MBOX_REQUEST;
	msg.tag_id = MBOX_TAG_GETCLKRATE;
	msg.tag_buff_size = 8;
	msg.tag_req_resp_code = 0;
	msg.tag_buff[0] = 0x2;
	msg.tag_buff[1] = 0x0;
	msg.tag_buff[2] = MBOX_TAG_LAST;
	
	mbox_call(MBOX_CH_PROP,&msg);
	if(msg.req_resp_code == 0x80000000){
		printk("Clock Rate %d Hz\n",msg.tag_buff[1]);
	}
#endif
#if defined(RNG_SUPPORT)
	rng_init();
#endif
#if defined(RASPI3)
	printk("RPi3 Booting EL:%d Clock Rate:%d MHz\n",get_current_EL(),msg.tag_buff[1]/(1000*1000));
#elif defined(VEXP_A9)
	printk("VEXP-A9 Booting\n");
#endif
	return 0;
}
