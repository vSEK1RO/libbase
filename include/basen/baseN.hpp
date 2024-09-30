#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace baseN
{
    /**
     * @param digits char[base] array of digits
     * @param digits_size size of digits array. Equals to base
     * @param map [out] uint8_t[256] array, where at an index equal to the value of the symbol is the index of this symbol in the digits array. 255 if there is no symbol
     * @throw basen::Exception(ALPH_COLLISION) if alphabet contain same chars
     */
    void digitsMap(const char *digits, uint8_t digits_size, uint8_t *map);
    /**
     * @param str [in] pointer to string
     * @param str_size
     * @param map uint8_t[256] array, where at an index equal to the value of the symbol is the index of this symbol in the digits array. 255 if there is no symbol
     * @return that string doesn't contain unnecessary symbols
     */
    bool isValid(const char *str, size_t str_size, const uint8_t *map) noexcept;
    /**
     * @param str string or string_view which you want to decode
     * @param map uint8_t[256] array, where at an index equal to the value of the symbol is the index of this symbol in the digits array. 255 if there is no symbol
     * @return that string doesn't contain unnecessary symbols
     */
    bool isValid(std::string_view str, const uint8_t *map) noexcept;

    /**
     * @param data vector or span of data which you want to encode
     * @param base from 1 to 255
     * @return estimated size after encoding
     * @throw basen::Exception(BASE) if base < 2 || base > 254
     */
    size_t sizeEncoded(std::span<const uint8_t> data, uint8_t base);
    /**
     * @param str string or string_view which you want to decode
     * @param base from 1 to 255
     * @param digits char[base] array of digits
     * @return estimated size after decoding
     * @throw basen::Exception(BASE) if base < 2 || base > 254
     * @throw basen::Exception(OVERFLOW) if if there is an overflow
     */
    size_t sizeDecoded(std::string_view str, uint8_t base, const char *digits);

    /**
     * @param data [in] pointer to data which you want encode
     * @param data_size
     * @param str [out] pointer to string for encoded data output
     * @param str_size
     * @param base from 1 to 255
     * @param digits char[base] array of digits
     * @code{cpp}
     * std::vector<uint8_t> data;
     * std::string str(baseN::sizeEncoded(data, 58), ' ');
     *
     * auto offset = baseN::encode(data.data(), data.size(), str.data(), str.size(), 58, base58::digits);
     * // deleting leading zeroes
     * str.erase(str.begin(), str.begin() + offset);
     * @endcode
     * @return number of leading chars, which should be trimmed
     * @throw basen::Exception(BASE) if base < 2 || base > 254
     * @warning contain leading zeros, returns count of them
     */
    size_t encode(const uint8_t *data, size_t data_size, char *str, size_t str_size, uint8_t base, const char *digits);
    /**
     * @param data vector or span of data which you want to encode
     * @param base from 1 to 255
     * @param digits char[base] array of digits
     * @code{cpp}
     * std::vector<uint8_t> data;
     * auto str = baseN::encode(data, 58, base58::digits);
     * @endcode
     * @return encoded string
     */
    std::string encode(std::span<const uint8_t> data, uint8_t base, const char *digits);

    /**
     * @param str [in] pointer to string which you want decode
     * @param str_size
     * @param data [out] pointer to data for encoded string output
     * @param data_size
     * @param base from 1 to 255
     * @param digits char[base] array of digits
     * @param map uint8_t[256] array, where at an index equal to the value of the symbol is the index of this symbol in the digits array. 255 if there is no symbol
     * @code{cpp}
     * std::string str;
     * std::vector<uint8_t> data(baseN::sizeDecoded(str, 58));
     *
     * auto offset = baseN::decode(str.data(), str.size(), data.data(), data.size(), 58, base58::digits, base58::map);
     * // deleting leading zeroes
     * data.erase(data.begin(), data.begin() + offset);
     * @endcode
     * @return number of leading chars, which should be trimmed
     * @throw basen::Exception(BASE) if base < 2 || base > 254
     * @throw basen::Exception(OUT_OF_ALPH) if out of alphabet
     * @warning contain leading zeros, returns count of them
     */
    size_t decode(const char *str, size_t str_size, uint8_t *data, size_t data_size, uint8_t base, const char *digits, const uint8_t *map);
    /**
     * @param str string or string_view which you want to decode
     * @param base from 1 to 255
     * @param digits char[base] array of digits
     * @param map uint8_t[256] array, where at an index equal to the value of the symbol is the index of this symbol in the digits array. 255 if there is no symbol
     * @code{cpp}
     * std::string str;
     * auto data = baseN::decode(str, 58, base58::digits, base58::map);
     * @endcode
     * @return decoded data
     */
    std::vector<uint8_t> decode(std::string_view str, uint8_t base, const char *digits, const uint8_t *map);
}