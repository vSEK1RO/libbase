#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace baseN
{
    bool isValid(const char *str, uint64_t str_size, const int8_t *map) noexcept;
    bool isValid(std::string_view str, const int8_t *map) noexcept;

    void encode(const uint8_t *data, uint64_t data_size, char *str, uint8_t base, const char *digits, uint64_t enc_size) noexcept;
    std::string encode(std::span<const uint8_t> data, uint8_t base, const char *digits, uint64_t enc_size) noexcept;
    std::string encode(std::span<const uint8_t> data, uint8_t base, const char *digits) noexcept;

    void decode(const char *str, uint8_t *data, uint64_t data_size, uint8_t base, const char *digits, const int8_t *map, uint64_t dec_size);
    std::vector<uint8_t> decode(std::string_view str, uint8_t base, const char *digits, const int8_t *map, uint64_t dec_size);
    std::vector<uint8_t> decode(std::string_view str, uint8_t base, const char *digits, const int8_t *map);
}