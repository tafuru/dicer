#include "dicer/encoding.hpp"

#include <climits>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <vector>

namespace {

constexpr std::size_t kBitsPerByte = CHAR_BIT;
constexpr std::size_t kShiftAmount = kBitsPerByte - 1;
constexpr std::uint8_t kContinuationBit = 0b1000'0000U;

}  // namespace

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

template <std::unsigned_integral UInt>
auto DecodeVarint(const std::vector<std::byte>& buffer) -> std::optional<UInt> {
  constexpr std::uint8_t kValueMask = 0b0111'1111U;

  UInt decoded_value{};
  std::size_t shift{};

  for (const auto& byte : buffer) {
    decoded_value |= (static_cast<UInt>(byte) & kValueMask) << shift;
    shift += kShiftAmount;

    if (!(static_cast<UInt>(byte) & kContinuationBit)) {
      return decoded_value;
    }

    if (shift >= sizeof(UInt) * kBitsPerByte) {
      return std::nullopt;
    }
  }

  return std::nullopt;
}
template auto DecodeVarint(const std::vector<std::byte>& buffer) -> std::optional<std::uint8_t>;
template auto DecodeVarint(const std::vector<std::byte>& buffer) -> std::optional<std::uint16_t>;
template auto DecodeVarint(const std::vector<std::byte>& buffer) -> std::optional<std::uint32_t>;
template auto DecodeVarint(const std::vector<std::byte>& buffer) -> std::optional<std::uint64_t>;

}  // namespace dicer
