#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include <stdint.h>

/*

Circular Buffer(Ring buffer): is a fixed-size data structure that uses a single,
contiguous block of memory to store data in a way that wraps around once the end
of the buffer is reached. It's typically used in situations where the buffer
needs to act as a FIFO (First-In, First-Out) queue.

Key Features:
        Fixed Size: The buffer has a predefined size, and it does not grow
dynamically.

        Circular Nature: When the buffer reaches the end, it wraps back to the
beginning if there is available space, thus behaving in a circular manner.

        Two Pointers/Indices: Circular buffers typically maintain two pointers:
                Head: Points to the position where data will be removed (for
reading). Tail: Points to the position where data will be inserted (for
writing).

        Overwrite or Block: Depending on the implementation, when the buffer is
full: It may overwrite the oldest data with new data. Or it may block new data
until some old data is removed.

Operations:
        Enqueue (Write): Add an item to the buffer at the position indicated by
the tail. After the item is added, the tail is incremented. Dequeue (Read):
Remove an item from the buffer at the position indicated by the head. After the
item is removed, the head is incremented. Wrap-around: Both head and tail
pointers wrap around to the beginning of the buffer when they reach the end.
*/

#define RINGBUF_MAX_LEN 50

typedef struct {
  int buf[RINGBUF_MAX_LEN];
  int head; // data is dequeued from head
  int tail; // data is enqueued at tail
  int count;
  int size;
} ringbuf_t;

void init_buf(ringbuf_t *rb, int size);
int enqueue(ringbuf_t *rb, int data);
int dequeue(ringbuf_t *rb, int *data);
int isEmpty(ringbuf_t *rb);
int isFull(ringbuf_t *rb);

#endif // _RING_BUF_H_
