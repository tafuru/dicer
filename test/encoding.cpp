#include "dicer/encoding.hpp"

#include <doctest/doctest.h>

#include <cstdint>
#include <limits>
#include <type_traits>

TEST_CASE_TEMPLATE("Zigzag Encoding", Int, std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
  using UInt = std::make_unsigned_t<Int>;
  CHECK(dicer::ZigzagEncode(static_cast<Int>(0)) == static_cast<UInt>(0U));
  CHECK(dicer::ZigzagEncode(static_cast<Int>(-1)) == static_cast<UInt>(1U));
  CHECK(dicer::ZigzagEncode(static_cast<Int>(1)) == static_cast<UInt>(2U));
  CHECK(dicer::ZigzagEncode(static_cast<Int>(-2)) == static_cast<UInt>(3U));
  CHECK(dicer::ZigzagEncode(std::numeric_limits<Int>::max()) == std::numeric_limits<UInt>::max() - 1U);
  CHECK(dicer::ZigzagEncode(std::numeric_limits<Int>::min()) == std::numeric_limits<UInt>::max());
}

TEST_CASE_TEMPLATE("Zigzag Decoding", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
  using Int = std::make_signed_t<UInt>;
  CHECK(dicer::ZigzagDecode(static_cast<UInt>(0U)) == static_cast<Int>(0));
  CHECK(dicer::ZigzagDecode(static_cast<UInt>(1U)) == static_cast<Int>(-1));
  CHECK(dicer::ZigzagDecode(static_cast<UInt>(2U)) == static_cast<Int>(1));
  CHECK(dicer::ZigzagDecode(static_cast<UInt>(3U)) == static_cast<Int>(-2));
  CHECK(dicer::ZigzagDecode(std::numeric_limits<UInt>::max() - 1U) == std::numeric_limits<Int>::max());
  CHECK(dicer::ZigzagDecode(std::numeric_limits<UInt>::max()) == std::numeric_limits<Int>::min());
}
