#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

#include <basen/baseN.hpp>

static constexpr auto log256 = std::log(256);

namespace baseN
{
    bool isValid(const char *str, uint64_t str_size, const int8_t *map) noexcept
    {
        return std::all_of(str, str + str_size, [map](char ch)
                           { return map[(int8_t)ch] != -1; });
    }
    bool isValid(std::string_view str, const int8_t *map) noexcept
    {
        return baseN::isValid(str.data(), str.size(), map);
    }
    uint64_t sizeEncoded(std::span<const uint8_t> data, uint8_t base)
    {
        std::span<const uint8_t> dv(std::find_if(data.begin(), data.end(), [](uint8_t item)
                                                 { return item != 0; }),
                                    data.end());
        if (dv.size() > std::numeric_limits<uint64_t>::max() / log256)
        {
            throw std::overflow_error("baseN::sizeEncoded: overflow");
        }
        return dv.size() * log256 / std::log(base) + 1 + (data.size() - dv.size());
    }
    uint64_t sizeDecoded(std::string_view str, uint8_t base, const char *digits) noexcept
    {
        std::string_view sv(std::find_if(str.begin(), str.end(), [digits](uint8_t ch)
                                         { return ch != digits[0]; }),
                            str.end());
        return sv.size() * std::log(base) / log256 + 1 + (str.size() - sv.size());
    }
    uint64_t encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size, uint8_t base, const char *digits)
    {
        std::vector<uint8_t> dv(std::find_if(data, data + data_size, [](uint8_t item)
                                             { return item != 0; }),
                                data + data_size);
        std::span<char> sv(str, str_size);
        auto sv_it = sv.rbegin();
        auto dv_it = dv.begin();
        auto quo_it = dv.begin();
        auto quo_it_last = dv.end();

        if (dv.size() != 0)
        {
            uint16_t div = *dv_it++;
            while ((dv[0] > base || quo_it_last > dv.begin() + 1) && sv_it < sv.rend() - 1)
            {
                if (div < base)
                {
                    div <<= 8;
                    div += *dv_it++;
                }
                *quo_it++ = div / base;
                div %= base;
                while (dv_it < quo_it_last)
                {
                    div <<= 8;
                    div += *dv_it++;
                    *quo_it++ = div / base;
                    div %= base;
                }
                quo_it_last = quo_it;
                dv_it = dv.begin();
                quo_it = dv.begin();
                *sv_it++ = digits[div];
                div = *dv_it++;
            }
            *sv_it++ = digits[div];
        }
        for (uint64_t i = 0; i < data_size - dv.size() && sv_it < sv.rend(); i++)
        {
            *sv_it++ = digits[0];
        }
        return std::distance(sv_it, sv.rend());
    }
    std::string encode(std::span<const uint8_t> data, uint8_t base, const char *digits) noexcept
    {
        std::string str(baseN::sizeEncoded(data, base), ' ');
        uint64_t offset = baseN::encode(data.data(), data.size(), str.data(), str.size(), base, digits);
        str.erase(str.begin(), str.begin() + offset);
        return str;
    }
    uint64_t decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size, uint8_t base, const char *digits, const int8_t *map)
    {
        std::string_view sv(std::find_if(str, str + str_size, [digits](char ch)
                                         { return ch != digits[0]; }),
                            str + str_size);
        if (!baseN::isValid(sv, map))
        {
            throw std::logic_error("baseN::decode: out of digits map");
        }
        std::span<uint8_t> dv(data, data_size);
        auto sv_it = sv.begin();
        auto quo_it = dv.rbegin();
        auto quo_it_last = dv.rbegin();
        uint16_t div;

        if (sv.size() != 0)
        {
            quo_it_last++;
            *quo_it = map[(int8_t)*sv_it++];
            while (sv_it < sv.end())
            {
                div = map[(int8_t)*sv_it++];
                while (quo_it < quo_it_last && quo_it < dv.rend() - 1)
                {
                    div += *quo_it * base;
                    *quo_it++ = div;
                    div >>= 8;
                }
                if (div != 0)
                {
                    *quo_it++ = div;
                }
                quo_it_last = quo_it;
                quo_it = dv.rbegin();
            }
        }
        for (uint64_t i = 0; i < str_size - sv.size() && quo_it_last < dv.rend(); i++)
        {
            *quo_it_last++ = 0;
        }
        return std::distance(quo_it_last, dv.rend());
    }
    std::vector<uint8_t> decode(std::string_view str, uint8_t base, const char *digits, const int8_t *map) noexcept
    {
        std::vector<uint8_t> data(baseN::sizeDecoded(str, base, digits));
        uint64_t offset = baseN::decode(str.data(), str.size(), data.data(), data.size(), base, digits, map);
        data.erase(data.begin(), data.begin() + offset);
        return data;
    }
}