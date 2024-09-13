#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <base/baseN.hpp>

namespace baseN
{
    bool isValid(const char *str, const int8_t *map) noexcept
    {
        std::string_view sv(str);
        for (size_t i = 0; i < sv.size(); i++)
        {
            if (map[(int8_t)str[i]] == -1)
            {
                return false;
            }
        }
        return true;
    }
    bool isValid(const std::string &str, const int8_t *map) noexcept
    {
        return baseN::isValid(str.data(), map);
    }

    void encode(const uint8_t *data, uint64_t data_size, char *str, uint8_t base, const char *digits, uint64_t enc_size) noexcept
    {
        if (data_size == 0)
        {
            return;
        }
        char res_str[enc_size];
        uint8_t div_buf[data_size];
        std::copy(data, data + data_size, div_buf);
        int64_t
            zero_count = 0,
            idx_div = 0,
            idx_quo = 0,
            idx_quo_last = data_size,
            idx_str = enc_size - 1;
        uint16_t div = data[idx_div++];

        while (zero_count < (int64_t)data_size && data[zero_count] == 0)
        {
            zero_count++;
        }
        while (idx_quo_last > 1 || div_buf[0] > base)
        {
            if (div < base)
            {
                div <<= 8;
                div += div_buf[idx_div++];
            }
            div_buf[idx_quo++] = div / base;
            div %= base;
            while (idx_div < idx_quo_last)
            {
                div <<= 8;
                div += div_buf[idx_div++];
                div_buf[idx_quo++] = div / base;
                div %= base;
            }
            idx_quo_last = idx_quo;
            idx_quo = 0;
            idx_div = 0;
            res_str[idx_str--] = digits[div];
            div = div_buf[idx_div++];
        }
        res_str[idx_str--] = digits[div];
        while (zero_count > 0 && idx_str >= 0)
        {
            res_str[idx_str--] = digits[0];
            zero_count--;
        }
        while (idx_str >= 0)
        {
            res_str[idx_str--] = ' ';
        }
        std::copy(res_str, res_str + enc_size, str);
    }
    std::string encode(std::vector<uint8_t> data, uint8_t base, const char *digits, uint64_t enc_size) noexcept
    {
        std::string str(enc_size, ' ');
        baseN::encode(data.data(), data.size(), str.data(), base, digits, enc_size);
        str.erase(str.begin(), std::find_if(
            str.begin(), str.end(), [](char ch){
                return ch != ' ';
            })
        );
        return str;
    }
    std::string encode(std::vector<uint8_t> data, uint8_t base, const char *digits) noexcept
    {
        return baseN::encode(data, base, digits, data.size() * std::log(256) / std::log(base) + 1);
    }

    void decode(const char *str, uint8_t *data, uint64_t data_size, uint8_t base, const char *digits, const int8_t *map, uint64_t dec_size)
    {
        if (str[0] == '\0')
        {
            return;
        }
        if (!baseN::isValid(str, map))
        {
            throw std::logic_error("baseN::decode: out of digits map");
        }
        uint8_t res_data[dec_size];
        uint64_t idx_str = 0;
        int64_t
            zero_count = 0,
            idx_quo = dec_size - 1,
            idx_quo_last = dec_size - 2;
        uint16_t div;

        while (str[zero_count] != '\0' && str[zero_count] == digits[0])
        {
            zero_count++;
        }
        res_data[idx_quo] = map[(int8_t)str[idx_str++]];
        while (str[idx_str] != '\0')
        {
            div = map[(int8_t)str[idx_str++]];
            while (idx_quo > idx_quo_last && idx_quo > 0)
            {
                div += res_data[idx_quo] * base;
                res_data[idx_quo--] = div;
                div >>= 8;
            }
            res_data[idx_quo--] = div;
            idx_quo_last = idx_quo;
            idx_quo = dec_size - 1;
        }
        std::copy(res_data, res_data + std::min(dec_size, data_size), data);
    }
    std::vector<uint8_t> decode(const std::string &str, uint8_t base, const char *digits, const int8_t *map, uint64_t dec_size)
    {
        std::vector<uint8_t> data(dec_size);
        baseN::decode(str.data(), data.data(), data.size(), base, digits, map, dec_size);
        data.erase(data.begin(), std::find_if(
            data.begin(), data.end(), [](uint8_t byte){
                return byte != 0;
            })
        );
        return data;
    }
    std::vector<uint8_t> decode(const std::string &str, uint8_t base, const char *digits, const int8_t *map)
    {
        return baseN::decode(str, base, digits, map, str.size() * std::log(base) / std::log(256) + 1);
    }
}