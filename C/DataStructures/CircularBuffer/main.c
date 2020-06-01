#include<stdio.h>
#include"circular_buf.h"


void disp(circ_buf_t *c,int ret){
    printf("head=%d tail=%d ret=%d\n",c->head,c->tail,ret);

}

int main()
{
    circ_buf_t *c;
    int ret=0;
    uint8_t data;
    c=init_buf(2);
    disp(c,ret);
    ret=push(c,3);
    disp(c,ret);
    ret=push(c,3);
    disp(c,ret);
    ret=push(c,3);
    disp(c,ret);
    ret=pop(c,&data);
    disp(c,ret);

    return 0;

}
