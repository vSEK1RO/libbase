#include <stdexcept>

#include <base/baseN.hpp>

namespace baseN
{
    bool isValid(const char *str, const int8_t *map) noexcept
    {
        uint8_t i = 0;
        while (str[i] != '\0')
        {
            if (map[(int8_t)str[i]] == -1)
            {
                return false;
            }
            i++;
        }
        return true;
    }
    bool isValid(const std::string &str, const int8_t *map) noexcept
    {
        return baseN::isValid(str.data(), map);
    }
    std::string encode(std::vector<uint8_t> data, uint8_t base, uint64_t enc_size, const char *digits) noexcept
    {
        if (data.size() == 0)
        {
            return "";
        }
        std::string str(enc_size, ' ');
        int64_t
            zero_count = 0,
            idx_div = 0,
            idx_quo = 0,
            idx_quo_last = data.size(),
            idx_str = str.size() - 1;
        uint16_t div = data[idx_div++];

        while (data[zero_count] == 0)
        {
            zero_count++;
        }
        while (idx_quo_last > 1 || data[0] > base)
        {
            if (div < base)
            {
                div <<= 8;
                div += data[idx_div++];
            }
            data[idx_quo++] = div / base;
            div %= base;
            while (idx_div < idx_quo_last)
            {
                div <<= 8;
                div += data[idx_div++];
                data[idx_quo++] = div / base;
                div %= base;
            }
            idx_quo_last = idx_quo;
            idx_quo = 0;
            idx_div = 0;
            str[idx_str--] = digits[div];
            div = data[idx_div++];
        }
        str[idx_str--] = digits[div];
        while (zero_count > 0 && idx_str > 0)
        {
            str[idx_str--] = '1';
            zero_count--;
        }
        str.erase(0, idx_str + 1);
        return str;
    }
    std::vector<uint8_t> decode(const std::string &str, uint8_t base, uint64_t dec_size, const char *digits, const int8_t *map)
    {
        if (str.size() == 0)
        {
            return std::vector<uint8_t>();
        }
        if (!baseN::isValid(str, map))
        {
            throw std::logic_error("baseN::decode: out of digits map");
        }
        std::vector<uint8_t> data(dec_size);
        uint64_t idx_str = 0;
        int64_t
            zero_count = 0,
            idx_quo = data.size() - 1,
            idx_quo_last = data.size() - 2;
        uint16_t div;

        while (str[zero_count] == digits[0])
        {
            zero_count++;
        }
        data[idx_quo] = map[(int8_t)str[idx_str++]];
        while (idx_str < str.size())
        {
            div = map[(int8_t)str[idx_str++]];
            while (idx_quo > idx_quo_last && idx_quo > 0)
            {
                div += data[idx_quo] * base;
                data[idx_quo--] = div;
                div >>= 8;  
            }
            data[idx_quo--] = div;
            idx_quo_last = idx_quo;
            idx_quo = data.size() - 1;
        }
        idx_quo = 0;
        while (data[idx_quo] == 0)
        {
            idx_quo++;
        }
        data.erase(data.begin(), data.begin() + idx_quo - zero_count);
        return data;
    }
}