#include "dicer/encoding.hpp"

#include <climits>
#include <concepts>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <vector>

namespace dicer {

template <std::signed_integral Int>
auto EncodeZigZag(Int value) -> std::make_unsigned_t<Int> {
  return static_cast<std::make_unsigned_t<Int>>((value << 1) ^ (value >> std::numeric_limits<Int>::digits));
}
template auto EncodeZigZag<std::int8_t>(std::int8_t value) -> std::make_unsigned_t<std::int8_t>;
template auto EncodeZigZag<std::int16_t>(std::int16_t value) -> std::make_unsigned_t<std::int16_t>;
template auto EncodeZigZag<std::int32_t>(std::int32_t value) -> std::make_unsigned_t<std::int32_t>;
template auto EncodeZigZag<std::int64_t>(std::int64_t value) -> std::make_unsigned_t<std::int64_t>;

template <std::unsigned_integral UInt>
auto DecodeZigZag(UInt value) -> std::make_signed_t<UInt> {
  return (value >> 1) ^ -(value & 1);
}
template auto DecodeZigZag<std::uint8_t>(std::uint8_t value) -> std::make_signed_t<std::uint8_t>;
template auto DecodeZigZag<std::uint16_t>(std::uint16_t value) -> std::make_signed_t<std::uint16_t>;
template auto DecodeZigZag<std::uint32_t>(std::uint32_t value) -> std::make_signed_t<std::uint32_t>;
template auto DecodeZigZag<std::uint64_t>(std::uint64_t value) -> std::make_signed_t<std::uint64_t>;

template <std::unsigned_integral UInt>
auto EncodeVarint(UInt value) -> std::vector<std::byte> {
  constexpr auto kBitsPerByte = CHAR_BIT;
  constexpr auto kShiftAmount = 7;
  constexpr auto kContinuationBit = 0x80U;
  std::vector<std::byte> buffer;
  buffer.reserve((kBitsPerByte * sizeof(UInt) + kShiftAmount - 1) / kShiftAmount);
  while (value >= kContinuationBit) {
    buffer.push_back(static_cast<std::byte>(value) | static_cast<std::byte>(kContinuationBit));
    value >>= kShiftAmount;
  }
  buffer.push_back(static_cast<std::byte>(value));
  return buffer;
}
template auto EncodeVarint(std::uint8_t value) -> std::vector<std::byte>;
template auto EncodeVarint(std::uint16_t value) -> std::vector<std::byte>;
template auto EncodeVarint(std::uint32_t value) -> std::vector<std::byte>;
template auto EncodeVarint(std::uint64_t value) -> std::vector<std::byte>;

}  // namespace dicer
