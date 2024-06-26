#include "dicer/encoding/zig_zag.hpp"

#include <concepts>
#include <limits>
#include <type_traits>

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

}  // namespace dicer
