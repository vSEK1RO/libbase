#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace base64
{
    extern const char digits[65];
    extern const uint8_t map[256];

    bool isValid(const char *str, size_t str_size) noexcept;
    bool isValid(std::string_view str) noexcept;

    /**
     * @throw std::overflow_error if if there is an overflow
     */
    size_t sizeEncoded(std::span<const uint8_t> data);
    size_t sizeDecoded(std::string_view str) noexcept;

    /**
     * @throw std::length_error if not enough allocated length
     * @warning contain leading zeros, returns count of them
     */
    void encode(const uint8_t *data, size_t data_size, char *str, size_t str_size);
    std::string encode(std::span<const uint8_t> data) noexcept;

    /**
     * @throw std::length_error if not enough allocated length
     * @throw std::logic_error if out of digits map
     * @throw std::logic_error if incorrect padding
     * @warning contain leading zeros, returns count of them
     */
    void decode(const char *str, size_t str_size, uint8_t *data, size_t data_size);
    std::vector<uint8_t> decode(std::string_view str) noexcept;
}