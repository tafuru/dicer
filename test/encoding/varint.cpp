#include <doctest/doctest.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

#include "dicer/dicer.hpp"  // NOLINT(misc-include-cleaner)

TEST_SUITE("Varint Encoding") {
  TEST_CASE_TEMPLATE("Encodes Zero", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(0U) == std::vector{std::byte{0b0000'0000U}});
  }

  TEST_CASE_TEMPLATE("Encodes Single Byte", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(0b0111'1111U) == std::vector{std::byte{0b0111'1111U}});
  }

  TEST_CASE_TEMPLATE("Encodes Multiple Bytes", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(0b0000'0001'0010'1100U) ==
          std::vector{std::byte{0b1010'1100U}, std::byte{0b0000'0010U}});
  }

  TEST_CASE_TEMPLATE("Encodes Edge Cases", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(0b1000'0000U) == std::vector{std::byte{0b1000'0000U}, std::byte{0b0000'0001U}});
    CHECK(dicer::EncodeVarint<UInt>(0b0100'0000'0000'0000U) ==
          std::vector{std::byte{0b1000'0000U}, std::byte{0b1000'0000U}, std::byte{0b0000'0001U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt8 Max", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(std::numeric_limits<std::uint8_t>::max()) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b0000'0001U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt16 Max", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(std::numeric_limits<std::uint16_t>::max()) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b0000'0011U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt32 Max", UInt, std::uint32_t, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(std::numeric_limits<std::uint32_t>::max()) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b1111'1111U}, std::byte{0b0000'1111U}});
  }

  TEST_CASE_TEMPLATE("Encodes UInt64 Max", UInt, std::uint64_t) {
    CHECK(dicer::EncodeVarint<UInt>(std::numeric_limits<std::uint64_t>::max()) ==
          std::vector{std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                      std::byte{0b0000'0001U}});
  }
}

TEST_SUITE("Varint Decoding") {
  TEST_CASE_TEMPLATE("Decodes Zero", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b0000'0000U}}) == 0U);
  }

  TEST_CASE_TEMPLATE("Decodes Single Byte", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b0111'1111U}}) == 0b0111'1111U);
  }

  TEST_CASE_TEMPLATE("Decodes Multiple Bytes", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1010'1100U}, std::byte{0b0000'0010U}}) == 0b0000'0001'0010'1100U);
  }

  TEST_CASE_TEMPLATE("Decodes Edge Cases", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1000'0000U}, std::byte{0b0000'0001U}}) == 0b1000'0000U);
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1000'0000U}, std::byte{0b1000'0000U}, std::byte{0b0000'0001U}}) ==
          0b0100'0000'0000'0000U);
  }

  TEST_CASE_TEMPLATE("Decodes UInt8 Max", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1111'1111U}, std::byte{0b0000'0001U}}) ==
          std::numeric_limits<std::uint8_t>::max());
  }

  TEST_CASE_TEMPLATE("Decodes UInt16 Max", UInt, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b0000'0011U}}) ==
          std::numeric_limits<std::uint16_t>::max());
  }

  TEST_CASE_TEMPLATE("Decodes UInt32 Max", UInt, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b1111'1111U}, std::byte{0b0000'1111U}}) ==
          std::numeric_limits<std::uint32_t>::max());
  }

  TEST_CASE_TEMPLATE("Decodes UInt64 Max", UInt, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b0000'0001U}}) == std::numeric_limits<std::uint64_t>::max());
  }

  TEST_CASE_TEMPLATE("Decodes Empty Buffer", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({}) == std::nullopt);
  }

  TEST_CASE_TEMPLATE("Decodes Invalid Buffer", UInt, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1000'0000U}}) == std::nullopt);
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1000'0000U}, std::byte{0b1000'0000U}}) == std::nullopt);
    CHECK(dicer::DecodeVarint<UInt>({std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b1111'1111U}, std::byte{0b1111'1111U}, std::byte{0b1111'1111U},
                                     std::byte{0b1111'1111U}, std::byte{0b0000'0001U}}) == std::nullopt);
  }
}
