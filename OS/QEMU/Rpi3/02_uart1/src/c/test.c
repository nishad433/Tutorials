#include<stdio.h>
#include <stddef.h>

#include"uart.h"
#include"gpio.h"


#define f(s_name, name) do { \
	printf("%s->%-15s 0x%02lx\n",#s_name,#name,offsetof(s_name, name));\
}while(0);

#define uart0_regs_print(name) 


void dislay(){
    f(uart0_regs,dr);
    f(uart0_regs,fr);
    f(uart0_regs,ibrd);
    f(uart0_regs,fbrd);
    f(uart0_regs,lcrh);
    f(uart0_regs,cr);
    f(uart0_regs,ifls);
    f(uart0_regs,imsc);
    f(uart0_regs,ris);
    f(uart0_regs,mis);
    f(uart0_regs,icr);
    f(uart0_regs,dmacr);
    f(uart0_regs,itcr);
    f(uart0_regs,itip);
    f(uart0_regs,itop);
    f(uart0_regs,tdr);
    printf("\n\n");    
    f(aux_regs_t,aux_irq);
    f(aux_regs_t,aux_enable);
    f(aux_regs_t,aux_mu_io);
    f(aux_regs_t,aux_mu_ier);
    f(aux_regs_t,aux_mu_iir);
    f(aux_regs_t,aux_mu_lcr);
    f(aux_regs_t,aux_mu_mcr);
    f(aux_regs_t,aux_mu_lsr);
    f(aux_regs_t,aux_mu_msr);
    f(aux_regs_t,aux_mu_scratch);
    f(aux_regs_t,aux_mu_cntl);
    f(aux_regs_t,aux_mu_stat);
    f(aux_regs_t,aux_mu_baud);
    
    printf("\n\n");    
    f(gpio_regs_t,gpfsel);
    f(gpio_regs_t,gpset);
    f(gpio_regs_t,gpclr);
    f(gpio_regs_t,gplev);
    f(gpio_regs_t,gpeds);
    f(gpio_regs_t,gpren);
    f(gpio_regs_t,gpfen);
    f(gpio_regs_t,gphen);
    f(gpio_regs_t,gplen);
    f(gpio_regs_t,gparen);
    f(gpio_regs_t,gpafen);
    f(gpio_regs_t,gppud);
    f(gpio_regs_t,gppudclk);
}

int main(){

    for(int i=0; i<55;i++){
	printf("gpio:%d FSEL_IDX=%d FSEL_SHIFT=%d idx=%d shift=%d\n",i,GPIO_TO_FSEL_ARRAY_IDX(i),GPIO_TO_FSEL_ARRAY_SHIFT(i),GPIO_TO_IDX(i),GPIO_TO_SHIFT(i));
    }
    return 0;
}
