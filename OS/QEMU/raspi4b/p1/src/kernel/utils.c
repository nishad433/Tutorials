#include <stddef.h>
#include <stdint.h>

extern uint64_t _text_end, _bss_start, _bss_end, _data_start, _data_end;

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

void clear_bss() {
  memset((void *)&_bss_start, 0,
         (int)((char *)&_bss_end - (char *)&_bss_start));
}

void copy_data_section() {
  memcpy((void *)&_data_start, (void *)&_text_end,
         (int)((char *)&_data_end - (char *)&_data_start));
}

int setup_c_environment(void) {
  clear_bss();
  copy_data_section();
  return 0;
}
