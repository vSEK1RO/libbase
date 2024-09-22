#include <algorithm>
#include <limits>
#include <stdexcept>

#include <basen/base64.hpp>
#include <basen/baseN.hpp>

namespace base64
{
    const char digits[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const int8_t map[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        //
    };
    bool isValid(const char *str, uint64_t str_size) noexcept
    {
        return base64::isValid(std::string_view(str, str_size));
    }
    bool isValid(std::string_view str) noexcept
    {
        std::string_view sv(str.begin(), std::find_if(str.rbegin(), str.rend(), [](char ch)
                                                      { return ch != '='; })
                                             .base());
        if (2 + sv.size() < str.size())
        {
            return false;
        }
        return baseN::isValid(sv, map);
    }
    uint64_t sizeEncoded(std::span<const uint8_t> data)
    {
        uint64_t str_size = data.size() / 3;
        if (str_size > std::numeric_limits<uint64_t>::max() / 4)
        {
            throw std::overflow_error("base64::sizeEncoded: overflow");
        }
        str_size = str_size * 4 + (data.size() % 3 ? 4 : 0);
        return str_size;
    }
    uint64_t sizeDecoded(std::string_view str) noexcept
    {
        auto size = std::distance(str.begin(), std::find_if(str.rbegin(), str.rend(), [](char ch)
                                                            { return ch != '='; })
                                                   .base());
        return size / 4 * 3 + (size % 4 ? size % 4 - 1 : 0);
    }
    void encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size)
    {
        if (str_size < base64::sizeEncoded(std::span<const uint8_t>(data, data_size)))
        {
            throw std::length_error("base64::encode: not enough allocated length");
        }
        for (uint64_t i = 0; i < data_size / 3; i++)
        {
            str[i * 4] = digits[data[i * 3] >> 2];
            str[i * 4 + 1] = digits[(data[i * 3] << 4 | data[i * 3 + 1] >> 4) & 0x3F];
            str[i * 4 + 2] = digits[(data[i * 3 + 1] << 2 | data[i * 3 + 2] >> 6) & 0x3F];
            str[i * 4 + 3] = digits[data[i * 3 + 2] & 0x3F];
        }
        uint64_t last_idx = data_size / 3 * 4;
        if (last_idx + 3 < str_size)
        {
            switch (data_size % 3)
            {
            case 1:
                str[last_idx] = digits[data[data_size - 1] >> 2];
                str[last_idx + 1] = digits[data[data_size - 1] << 4 & 0x30];
                str[last_idx + 2] = '=';
                str[last_idx + 3] = '=';
                break;
            case 2:
                str[last_idx] = digits[data[data_size - 2] >> 2];
                str[last_idx + 1] = digits[(data[data_size - 2] << 4 | data[data_size - 1] >> 4) & 0x3F];
                str[last_idx + 2] = digits[data[data_size - 1] & 0x0F];
                str[last_idx + 3] = '=';
                break;
            default:
                break;
            }
        }
    }
    std::string encode(std::span<const uint8_t> data) noexcept
    {
        std::string str(base64::sizeEncoded(data), ' ');
        base64::encode(data.data(), data.size(), str.data(), str.size());
        return str;
    }
    void decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size)
    {
        std::string_view sv(str, str_size);
        if (data_size < base64::sizeDecoded(sv))
        {
            throw std::length_error("base64::decode: not enough allocated length");
        }
        if (!base64::isValid(sv))
        {
            throw std::logic_error("base64::decode: out of digits map");
        }
        auto size = std::distance(sv.begin(), std::find_if(sv.rbegin(), sv.rend(), [](char ch)
                                                           { return ch != '='; })
                                                  .base());
        if (sv.size() % 4 != 0 || sv.size() - size > 2)
        {
            throw std::logic_error("base64::decode: incorrect padding");
        }
        for (auto i = 0; i < size / 4; i++)
        {
            data[i * 3] = map[(int8_t)str[i * 4]] << 2 | map[(int8_t)str[i * 4 + 1]] >> 4;
            data[i * 3 + 1] = map[(int8_t)str[i * 4 + 1]] << 4 | map[(int8_t)str[i * 4 + 2]] >> 2;
            data[i * 3 + 2] = map[(int8_t)str[i * 4 + 2]] << 6 | map[(int8_t)str[i * 4 + 3]];
        }
        uint64_t last_idx = size / 4 * 3;
        switch (size % 4)
        {
        case 2:
            data[last_idx] = map[(int8_t)str[size - 2]] << 2 | map[(int8_t)str[size - 1]] >> 4;
            break;
        case 3:
            data[last_idx] = map[(int8_t)str[size - 3]] << 2 | map[(int8_t)str[size - 2]] >> 4;
            data[last_idx + 1] = map[(int8_t)str[size - 2]] << 4 | map[(int8_t)str[size - 1]] >> 2;
            break;
        default:
            break;
        }
    }
    std::vector<uint8_t> decode(std::string_view str) noexcept
    {
        std::vector<uint8_t> data(base64::sizeDecoded(str));
        base64::decode(str.data(), str.size(), data.data(), data.size());
        return data;
    }
}