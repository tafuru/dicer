#ifndef DICER_ENCODING_HPP_
#define DICER_ENCODING_HPP_

#include <concepts>
#include <type_traits>

namespace dicer {

/**
 * @brief Encodes a signed integral value into an unsigned integral value using Zigzag encoding.
 * @tparam Int A type that is a signed integral.
 * @param value The signed integral value to be encoded.
 * @return The encoded value as an unsigned integral.
 */
template <std::signed_integral Int>
auto ZigzagEncode(Int value) -> std::make_unsigned_t<Int>;

/**
 * @brief Decodes an unsigned integral value into a signed integral value using Zigzag decoding.
 * @tparam UInt A type that is an unsigned integral.
 * @param value The unsigned integral value to be decoded.
 * @return The decoded value as a signed integral.
 */
template <std::unsigned_integral UInt>
auto ZigzagDecode(UInt value) -> std::make_signed_t<UInt>;

}  // namespace dicer

#endif  // DICER_ENCODING_HPP_
