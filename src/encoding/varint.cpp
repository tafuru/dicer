#include "dicer/encoding/varint.hpp"

#include <climits>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace {

constexpr std::size_t kBitsPerByte = CHAR_BIT;
constexpr std::size_t kShiftAmount = kBitsPerByte - 1;
constexpr std::uint8_t kContinuationBit = 0b1000'0000U;

}  // namespace

namespace dicer {

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
