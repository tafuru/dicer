#include "dicer/encoding.hpp"

#include <concepts>
#include <limits>
#include <type_traits>

namespace dicer {

template <std::signed_integral Int>
auto ZigzagEncode(Int value) -> std::make_unsigned_t<Int> {
  return static_cast<std::make_unsigned_t<Int>>((value << 1) ^ (value >> std::numeric_limits<Int>::digits));
}
template auto ZigzagEncode<std::int8_t>(std::int8_t value) -> std::make_unsigned_t<std::int8_t>;
template auto ZigzagEncode<std::int16_t>(std::int16_t value) -> std::make_unsigned_t<std::int16_t>;
template auto ZigzagEncode<std::int32_t>(std::int32_t value) -> std::make_unsigned_t<std::int32_t>;
template auto ZigzagEncode<std::int64_t>(std::int64_t value) -> std::make_unsigned_t<std::int64_t>;

template <std::unsigned_integral UInt>
auto ZigzagDecode(UInt value) -> std::make_signed_t<UInt> {
  return (value >> 1) ^ -(value & 1);
}
template auto ZigzagDecode<std::uint8_t>(std::uint8_t value) -> std::make_signed_t<std::uint8_t>;
template auto ZigzagDecode<std::uint16_t>(std::uint16_t value) -> std::make_signed_t<std::uint16_t>;
template auto ZigzagDecode<std::uint32_t>(std::uint32_t value) -> std::make_signed_t<std::uint32_t>;
template auto ZigzagDecode<std::uint64_t>(std::uint64_t value) -> std::make_signed_t<std::uint64_t>;

}  // namespace dicer
