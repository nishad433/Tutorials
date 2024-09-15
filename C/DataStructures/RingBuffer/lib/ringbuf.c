#include <ringbuf.h>
#include <stdlib.h>

void init_buf(ringbuf_t *rb, int size) {
  if (rb) {
    rb->count = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->size = size;
  }
}

int enqueue(ringbuf_t *rb, int data) {
  if (isFull(rb))
    return -1;

  rb->buf[rb->tail] = data;
  rb->tail = (rb->tail + 1) % rb->size;
  rb->count++;
  return 0;
}

int dequeue(ringbuf_t *rb, int *data) {
  if (isEmpty(rb) || !data)
    return -1;

  *data = rb->buf[rb->head];
  rb->head = (rb->head + 1) % rb->size;
  rb->count--;
  return 0;
}

int isEmpty(ringbuf_t *rb) { return (rb->count == 0); }

int isFull(ringbuf_t *rb) { return (rb->count == rb->size); }
