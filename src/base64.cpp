#include <algorithm>
#include <stdexcept>

#include <base/base64.hpp>
#include <base/baseN.hpp>

static const char b64digits[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int8_t b64map[] = {
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

namespace base64
{
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
        return baseN::isValid(sv, b64map);
    }
    void encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size)
    {
        if (str_size < data_size / 3 * 4 + (data_size % 3 ? 4 : 0))
        {
            throw std::logic_error("base64::encode: not enough allocated length");
        }
        for (uint64_t i = 0; i < data_size / 3; i++)
        {
            str[i * 4] = b64digits[data[i * 3] >> 2];
            str[i * 4 + 1] = b64digits[(data[i * 3] << 4 | data[i * 3 + 1] >> 4) & 0x3F];
            str[i * 4 + 2] = b64digits[(data[i * 3 + 1] << 2 | data[i * 3 + 2] >> 6) & 0x3F];
            str[i * 4 + 3] = b64digits[data[i * 3 + 2] & 0x3F];
        }
        uint64_t last_idx = data_size / 3 * 4;
        if (last_idx + 3 < str_size)
        {
            switch (data_size % 3)
            {
            case 1:
                str[last_idx] = b64digits[data[data_size - 1] >> 2];
                str[last_idx + 1] = b64digits[data[data_size - 1] << 4 & 0x30];
                str[last_idx + 2] = '=';
                str[last_idx + 3] = '=';
                break;
            case 2:
                str[last_idx] = b64digits[data[data_size - 2] >> 2];
                str[last_idx + 1] = b64digits[(data[data_size - 2] << 4 | data[data_size - 1] >> 4) & 0x3F];
                str[last_idx + 2] = b64digits[data[data_size - 1] & 0x0F];
                str[last_idx + 3] = '=';
                break;
            default:
                break;
            }
        }
    }
    std::string encode(std::span<const uint8_t> data) noexcept
    {
        std::string str(data.size() / 3 * 4 + (data.size() % 3 ? 4 : 0), ' ');
        base64::encode(data.data(), data.size(), str.data(), str.size());
        return str;
    }
    // void decode(const char *str, uint8_t *data, uint64_t data_size)
    // {
    // }
    // std::vector<uint8_t> decode(std::string_view str)
    // {
    // }
}