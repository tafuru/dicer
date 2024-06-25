#ifndef DICER_ENCODING_HPP_
#define DICER_ENCODING_HPP_

#include <concepts>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <vector>

namespace dicer {

/**
 * @brief Encodes a signed integral value into an unsigned integral value using ZigZag encoding.
 *
 * @tparam Int A type that is a signed integral.
 * @param value The signed integral value to be encoded.
 * @return The encoded value as an unsigned integral.
 */
template <std::signed_integral Int>
auto EncodeZigZag(Int value) -> std::make_unsigned_t<Int>;

/**
 * @brief Decodes an unsigned integral value into a signed integral value using ZigZag decoding.
 *
 * @tparam UInt A type that is an unsigned integral.
 * @param value The unsigned integral value to be decoded.
 * @return The decoded value as a signed integral.
 */
template <std::unsigned_integral UInt>
auto DecodeZigZag(UInt value) -> std::make_signed_t<UInt>;

/**
 * @brief Encodes an unsigned integer using Varint encoding.
 *
 * @tparam UInt An unsigned integral type.
 * @param value The unsigned integer value to encode.
 * @return A vector of bytes representing the Varint-encoded value.
 */
template <std::unsigned_integral UInt>
auto EncodeVarint(UInt value) -> std::vector<std::byte>;

/**
 * @brief Decodes a Varint-encoded value into an unsigned integer.
 *
 * @tparam UInt An unsigned integral type.
 * @param buffer A vector of bytes representing the Varint-encoded value.
 * @return An optional containing the decoded unsigned integer value, or std::nullopt if decoding fails.
 *
 * @note The function returns std::nullopt if the input is invalid (e.g., incomplete encoding or integer overflow).
 */
template <std::unsigned_integral UInt>
auto DecodeVarint(const std::vector<std::byte>& buffer) -> std::optional<UInt>;

}  // namespace dicer

#endif  // DICER_ENCODING_HPP_
