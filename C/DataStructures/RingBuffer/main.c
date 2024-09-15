#include "ringbuf.h"
#include <stdio.h>
#include <stdlib.h>

void disp(ringbuf_t *rb, int ret) {
  printf("head=%d tail=%d count=%d size=%d ret=%d\n", rb->head, rb->tail,
         rb->count, rb->size, ret);
}

int main() {
  int ret = 0;
  int data;
  ringbuf_t *rb = malloc(sizeof(ringbuf_t));

  if (!rb) {
    printf("Alloc failed\n");
    return -1;
  }

  init_buf(rb, RINGBUF_MAX_LEN);
  disp(rb, ret);
  ret = enqueue(rb, 3);
  disp(rb, ret);
  ret = enqueue(rb, 3);
  disp(rb, ret);
  ret = enqueue(rb, 3);
  disp(rb, ret);
  ret = dequeue(rb, &data);
  disp(rb, ret);

  return 0;
}
