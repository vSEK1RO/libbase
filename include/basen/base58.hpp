#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace base58
{
    /**
     * @brief bitcoin alphabet
     */
    extern const char digits[59];
    extern const uint8_t map[256];

    bool isValid(const char *str, size_t str_size) noexcept;
    bool isValid(std::string_view str) noexcept;

    /**
     * @throw basen::Exception(OVERFLOW) if there is an overflow
     */
    size_t sizeEncoded(std::span<const uint8_t> data);
    size_t sizeDecoded(std::string_view str) noexcept;

    /**
     * @return number of leading chars, which should be trimmed
     * @warning contain leading zeros, returns count of them
     */
    size_t encode(const uint8_t *data, size_t data_size, char *str, size_t str_size);
    std::string encode(std::span<const uint8_t> data);

    /**
     * @return number of leading chars, which should be trimmed
     * @warning contain leading zeros, returns count of them
     */
    size_t decode(const char *str, size_t str_size, uint8_t *data, size_t data_size);
    std::vector<uint8_t> decode(std::string_view str);

    /**
     * @param data vector or span of data which you want to encode
     * @return encoded string + 4 first bytes of double sha256
     */
    std::string encodeCheck(std::span<const uint8_t> data);
    /**
     * @param str string or string_view which you want to decode
     * @return decoded data without 4 first bytes of double sha256
     * @throw basen::Exception(PADDING) if str size < 4
     * @throw basen::Exception(CHECKSUM) checksum incorrect
     */
    std::vector<uint8_t> decodeCheck(std::string_view str);
}