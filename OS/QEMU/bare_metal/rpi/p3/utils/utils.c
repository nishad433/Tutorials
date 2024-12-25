#include <stddef.h>
#include <stdint.h>

void *memset(void *ptr, int c, size_t n) {
  for (int i = 0; i < n; i++) {
    ((char *)ptr)[i] = c;
  }
  return ptr;
}

void *memcpy(void *dest, const void *src, size_t n) {
  if (dest > src && (char *)dest < ((char *)src) + n) {
    for (int i = n - 1; i >= 0; i--) {
      ((char *)dest)[i] = ((char *)src)[i];
    }
  } else {
    for (int i = 0; i < n; i++) {
      ((char *)dest)[i] = ((char *)src)[i];
    }
  }
  return dest;
}
