#include<stdlib.h>
#include "circular_buf.h"

circ_buf_t* init_buf(int len){
        circ_buf_t *temp;
        temp=malloc(sizeof(circ_buf_t));
        temp->maxlen=len;
        temp->head=0;
        temp->tail=0;
        temp->buf=malloc(len*(sizeof(uint8_t)));
        return temp;
}

int push(circ_buf_t *c, uint8_t data){
        int next;

        next=(c->head+1)%(c->maxlen);

        if(next==c->tail)
                return -1;

        c->buf[c->head]=data;
        c->head=next;
        return 0;
}

int pop(circ_buf_t *c, uint8_t *data){
        if(is_empty(c))
                return -1;
        *data=c->buf[c->tail];
        c->tail=(c->tail+1)%(c->maxlen);
        return 0;
}

int is_empty(circ_buf_t *c){
        return (c->head==c->tail);
}

int is_full(circ_buf_t *c){
        int next;
        next=(c->head+1)%(c->maxlen);
        return (next==c->tail);
}

