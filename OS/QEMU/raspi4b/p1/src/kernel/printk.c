#include <printk.h>
#include <stdarg.h>
#include <uart.h>

static int int2str(int val, char *buffer) {
  int count = -1, digit;
  int neg = 0, i;
  char c;

  if (val < 0) {
    val = val * (-1);
    neg = 1;
  }

  do {
    digit = val % 10;
    buffer[++count] = digit + '0';
    val = val / 10;
  } while (val != 0);

  if (neg) {
    buffer[++count] = '-';
  }

  for (i = 0; i <= count / 2; i++) {
    c = buffer[i];
    buffer[i] = buffer[count - i];
    buffer[count - i] = c;
  }
  buffer[++count] = '\0';

  return count + 1;
}

static int hex2str(unsigned int val, char *buffer) {
  int count = -1, digit;
  int i;
  char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  char c;

  do {
    digit = val % 16;
    buffer[++count] = hexmap[digit];
    val = val / 16;
  } while (val != 0);

  for (i = 0; i <= count / 2; i++) {
    c = buffer[i];
    buffer[i] = buffer[count - i];
    buffer[count - i] = c;
  }
  buffer[++count] = '\0';

  return count + 1;
}

int printk(char *fmt, ...) {

  int count = -1, c, j;
  unsigned char *cc;
  char *ptr;
  char buffer[1024], b[20];
  va_list ap;

  va_start(ap, fmt);

  for (ptr = fmt; *ptr; ptr++) {

    if (*ptr == '%') {
      switch (*++ptr) {
      case 'd':
        c = int2str(va_arg(ap, int), b);
        for (j = 0; j < c - 1; j++) {
          buffer[++count] = b[j];
        }
        break;
      case 'x':
        c = hex2str(va_arg(ap, unsigned int), b);
        for (j = 0; j < c - 1; j++) {
          buffer[++count] = b[j];
        }
        break;
      case 's':
        cc = va_arg(ap, unsigned char *);
        while (*cc) {
          buffer[++count] = *cc++;
        }
        break;
      }
    } else {
      buffer[++count] = *ptr;
    }
  }

  va_end(ap);
  buffer[++count] = '\0';
  write(buffer);
  return count;
}
