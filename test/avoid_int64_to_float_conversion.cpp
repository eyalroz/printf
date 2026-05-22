#define PRINTF_VISIBILITY static
#define PRINTF_USE_DOUBLE_INTERNALLY 0
#define PRINTF_AVOID_INT64_TO_FLOAT_CONVERSION 1
#define PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS 0
#define PRINTF_SUPPORT_WRITEBACK_SPECIFIER 0

#include <printf/printf.c>

static void putchar_(char c)
{
  (void) c;
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <cstring>
#include <cstdint>

TEST_CASE("single precision can avoid int64 to float conversions")
{
  char buffer[64];

  STATIC_REQUIRE(sizeof(((struct floating_point_components*) 0)->integral) == sizeof(int32_t));
  STATIC_REQUIRE(sizeof(((struct floating_point_components*) 0)->fractional) == sizeof(int32_t));
  STATIC_REQUIRE(PRINTF_MAX_SUPPORTED_PRECISION == 9);

  sprintf_(buffer, "%.10f", 1.25);
  CHECK(std::strcmp(buffer, "1.2500000000") == 0);

  sprintf_(buffer, "%.18f", -67224.546875);
  CHECK(std::strcmp(buffer, "-67224.546875008000000000") == 0);
}
