#include <utils.h>

extern uint64_t _text_end, _bss_start, _bss_end, _data_start, _data_end;

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
