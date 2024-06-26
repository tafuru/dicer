#include <doctest/doctest.h>

#include <cstdint>
#include <limits>
#include <type_traits>

#include "dicer/dicer.hpp"  // NOLINT(misc-include-cleaner)

TEST_SUITE("ZigZag Encoding") {
  TEST_CASE_TEMPLATE("Encodes Zero", Int, std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
    CHECK(dicer::EncodeZigZag<Int>(0) == 0U);
  }

  TEST_CASE_TEMPLATE("Encodes Positive Numbers", Int, std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
    CHECK(dicer::EncodeZigZag<Int>(1) == 2U);
    CHECK(dicer::EncodeZigZag<Int>(2) == 4U);
    CHECK(dicer::EncodeZigZag<Int>(3) == 6U);
  }

  TEST_CASE_TEMPLATE("Encodes Negative Numbers", Int, std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
    CHECK(dicer::EncodeZigZag<Int>(-1) == 1U);
    CHECK(dicer::EncodeZigZag<Int>(-2) == 3U);
    CHECK(dicer::EncodeZigZag<Int>(-3) == 5U);
  }

  TEST_CASE_TEMPLATE("Encodes Extreme Cases", Int, std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
    using UInt = std::make_unsigned_t<Int>;
    CHECK(dicer::EncodeZigZag(std::numeric_limits<Int>::max()) == std::numeric_limits<UInt>::max() - 1U);
    CHECK(dicer::EncodeZigZag(std::numeric_limits<Int>::min()) == std::numeric_limits<UInt>::max());
  }
}

TEST_SUITE("ZigZag Decoding") {
  TEST_CASE_TEMPLATE("Decodes Zero", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeZigZag<UInt>(0U) == 0);
  }

  TEST_CASE_TEMPLATE("Decodes Odd Numbers", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint32_t) {
    CHECK(dicer::DecodeZigZag<UInt>(1U) == -1);
    CHECK(dicer::DecodeZigZag<UInt>(3U) == -2);
    CHECK(dicer::DecodeZigZag<UInt>(5U) == -3);
  }

  TEST_CASE_TEMPLATE("Decodes Even Numbers", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint32_t) {
    CHECK(dicer::DecodeZigZag<UInt>(2U) == 1);
    CHECK(dicer::DecodeZigZag<UInt>(4U) == 2);
    CHECK(dicer::DecodeZigZag<UInt>(6U) == 3);
  }

  TEST_CASE_TEMPLATE("Decodes Extreme Cases", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint32_t) {
    using Int = std::make_signed_t<UInt>;
    CHECK(dicer::DecodeZigZag(std::numeric_limits<UInt>::max() - 1U) == std::numeric_limits<Int>::max());
    CHECK(dicer::DecodeZigZag(std::numeric_limits<UInt>::max()) == std::numeric_limits<Int>::min());
  }
}
