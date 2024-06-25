#include "dicer/encoding.hpp"

#include <doctest/doctest.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

TEST_CASE_TEMPLATE("ZigZag Encoding", Int, std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
  using UInt = std::make_unsigned_t<Int>;
  CHECK(dicer::EncodeZigZag(static_cast<Int>(0)) == static_cast<UInt>(0U));
  CHECK(dicer::EncodeZigZag(static_cast<Int>(-1)) == static_cast<UInt>(1U));
  CHECK(dicer::EncodeZigZag(static_cast<Int>(1)) == static_cast<UInt>(2U));
  CHECK(dicer::EncodeZigZag(static_cast<Int>(-2)) == static_cast<UInt>(3U));
  CHECK(dicer::EncodeZigZag(std::numeric_limits<Int>::max()) == std::numeric_limits<UInt>::max() - 1U);
  CHECK(dicer::EncodeZigZag(std::numeric_limits<Int>::min()) == std::numeric_limits<UInt>::max());
}

TEST_CASE_TEMPLATE("ZigZag Decoding", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
  using Int = std::make_signed_t<UInt>;
  CHECK(dicer::DecodeZigZag(static_cast<UInt>(0U)) == static_cast<Int>(0));
  CHECK(dicer::DecodeZigZag(static_cast<UInt>(1U)) == static_cast<Int>(-1));
  CHECK(dicer::DecodeZigZag(static_cast<UInt>(2U)) == static_cast<Int>(1));
  CHECK(dicer::DecodeZigZag(static_cast<UInt>(3U)) == static_cast<Int>(-2));
  CHECK(dicer::DecodeZigZag(std::numeric_limits<UInt>::max() - 1U) == std::numeric_limits<Int>::max());
  CHECK(dicer::DecodeZigZag(std::numeric_limits<UInt>::max()) == std::numeric_limits<Int>::min());
}

TEST_SUITE("Varint Encoding") {
  TEST_CASE_TEMPLATE("Encodes Zero", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(0U)) == std::vector{std::byte{0b0000'0000U}});
  }

  TEST_CASE_TEMPLATE("Encodes Single Byte", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(0b0111'1111U)) == std::vector{std::byte{0b0111'1111U}});
  }

  TEST_CASE_TEMPLATE("Encodes Multiple Bytes", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(0b0000'0001'0010'1100U)) ==
          std::vector{std::byte{0b1010'1100U}, std::byte{0b0000'0010U}});
  }

  TEST_CASE_TEMPLATE("Encodes Edge Cases", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(0b1000'0000U)) ==
          std::vector{std::byte{0b1000'0000U}, std::byte{0b0000'0001U}});
    CHECK(dicer::EncodeVarint(static_cast<UInt>(0b0100'0000'0000'0000U)) ==
          std::vector{std::byte{0b1000'0000U}, std::byte{0b1000'0000U}, std::byte{0b0000'0001U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt8 Max", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(std::numeric_limits<std::uint8_t>::max())) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b0000'0001U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt16 Max", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(std::numeric_limits<std::uint16_t>::max())) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b0000'0011U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt32 Max", UInt, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(std::numeric_limits<std::uint32_t>::max())) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b1111'1111U}, std::byte{0b0000'1111U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt64 Max", UInt, std::uint64_t) {
    CHECK(dicer::EncodeVarint(static_cast<UInt>(std::numeric_limits<std::uint64_t>::max())) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b0000'0001U}});
  }
}
