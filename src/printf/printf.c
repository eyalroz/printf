#include <printf/printf.h>
#include "internal.h"

/*
 * We can't use putchar_ as is, since our output gadget
 * only takes pointers to functions with an extra argument
 */
static inline void putchar_wrapper(char c, void* unused)
{
  (void) unused;
  putchar_(c);
}

static inline output_gadget_t extern_putchar_gadget(void)
{
  return function_gadget(putchar_wrapper, NULL);
}


int vprintf_(const char* format, va_list arg)
{
  output_gadget_t gadget = extern_putchar_gadget();
  return eyalroz_vsnprintf_impl(&gadget, format, arg);
}

int printf_(const char *format, ...)
{
  int ret;
  va_list args;
  va_start(args, format);
  ret = vprintf_(format, args);
  va_end(args);
  return ret;
}
