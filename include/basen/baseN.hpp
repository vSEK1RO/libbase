#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace baseN
{
    bool isValid(const char *str, uint64_t str_size, const int8_t *map) noexcept;
    bool isValid(std::string_view str, const int8_t *map) noexcept;

    uint64_t sizeEncoded(std::span<const uint8_t> data, uint8_t base);
    uint64_t sizeDecoded(std::string_view str, uint8_t base, const char* digits) noexcept;

    /** 
     * @param data pointer to data which you want encode
     * @param data_size count of bytes to encode
     * @param str pointer to string for encoded data output
     * @param str_size str allocated size
     * @param base since 1, up to 256
     * @param digits char[base] array of digits
     * @code{cpp}
     * std::vector<uint8_t> data;
     * std::string str(baseN::sizeEncoded(data, 58));
     * 
     * auto offset = baseN::encode(data.data(), data.size(), str.data(), str.size(), 58, base58::digits);
     * // deleting leading zeroes
     * str.erase(str.begin(), str.begin() + offset);
     * @endcode
     * @return returns number of leading chars, which should be trimmed
     * @warning contain leading zeros, returns count of them
     */
    uint64_t encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size, uint8_t base, const char *digits);
    std::string encode(std::span<const uint8_t> data, uint8_t base, const char *digits) noexcept;

    uint64_t decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size, uint8_t base, const char *digits, const int8_t *map);
    std::vector<uint8_t> decode(std::string_view str, uint8_t base, const char *digits, const int8_t *map) noexcept;
}