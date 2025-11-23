#ifndef PRINTF_INTERNAL_H_
#define PRINTF_INTERNAL_H_

#include <printf/printf.h>
#include <limits.h>

#if !(defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L))
/* C90 */
#if defined(_MSC_VER)
#define inline __inline
#else
#define inline __inline__
#endif /* defined(_MSC_VER) */
#endif /* !(defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)) */

/*
 * The printf()-family functions return an `int`; it is therefore
 * unnecessary/inappropriate to use size_t - often larger than int
 * in practice - for non-negative related values, such as widths,
 * precisions, offsets into buffers used for printing and the sizes
 * of these buffers. instead, we use:
 */
typedef unsigned int printf_size_t;
#define PRINTF_MAX_POSSIBLE_BUFFER_SIZE INT_MAX
  /*
   * If we were to nitpick, this would actually be INT_MAX + 1,
   * since INT_MAX is the maximum return value, which excludes the
   * trailing '\0'.
   */

/*
 * wrapper (used as buffer) for output function type
 *
 * One of the following must hold:
 * 1. max_chars is 0
 * 2. buffer is non-null
 * 3. function is non-null
 *
 * ... otherwise bad things will happen.
 */
typedef struct {
  void (*function)(char c, void* extra_arg);
  void* extra_function_arg;
  char* buffer;
  printf_size_t pos;
  printf_size_t max_chars;
} output_gadget_t;

static inline output_gadget_t discarding_gadget(void)
{
  output_gadget_t gadget;
  gadget.function = NULL;
  gadget.extra_function_arg = NULL;
  gadget.buffer = NULL;
  gadget.pos = 0;
  gadget.max_chars = 0;
  return gadget;
}

static inline output_gadget_t function_gadget(void (*function)(char, void*), void* extra_arg)
{
  output_gadget_t result = discarding_gadget();
  result.function = function;
  result.extra_function_arg = extra_arg;
  result.max_chars = PRINTF_MAX_POSSIBLE_BUFFER_SIZE;
  return result;
}

int eyalroz_vsnprintf_impl(output_gadget_t* output, const char* format, va_list args);

#endif /* PRINTF_INTERNAL_H_ */
