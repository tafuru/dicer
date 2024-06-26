#ifndef DICER_ENCODING_VARINT_HPP_
#define DICER_ENCODING_VARINT_HPP_

#include <concepts>
#include <cstddef>
#include <optional>
#include <vector>

namespace dicer {

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

#endif  // DICER_ENCODING_VARINT_HPP_
