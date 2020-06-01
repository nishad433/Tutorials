#ifndef _CIRCULAR_BUF_H_
#define _CIRCULAR_BUF_H_

#include<stdint.h>


typedef struct{
    uint8_t * buf;
    int head;
    int tail;
    int maxlen;
}circ_buf_t;


circ_buf_t* init_buf(int len);
int push(circ_buf_t *c, uint8_t data);
int pop(circ_buf_t *c, uint8_t *data);
int is_empty(circ_buf_t *c);
int is_full(circ_buf_t *c);

#endif
