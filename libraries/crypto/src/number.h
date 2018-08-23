/*
 * MIT License
 *
 * Copyright (c) 2018 Quasis (info@quasis.io)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once
#include <assert.h>
#include <initializer_list>
#include "string.h"

namespace crypto
{
    template<size_t BITS, typename word_t = uint8_t>
    class Number
    {
        enum : size_t
        {
            WORD_BIT = CHAR_BIT * sizeof(word_t),
            BINS     = (size_t)(BITS / WORD_BIT),
            SIZE     = (size_t)(BITS / CHAR_BIT),
        };

        word_t              m_data [BINS];
        static inline const Number ZERO{};


     public:

        Number() : m_data{}
        {
        }


        Number(const std::initializer_list<word_t> &object)
        {
            memcpy(this->data(), object.begin(), object.size() * sizeof(word_t));
        }


        Number(const word_t *record, const size_t &length = BINS) : m_data{}
        {
            assert(this->size()       >= length * sizeof(word_t));
            memcpy(this->data(), record, length * sizeof(word_t));
        }


        template<size_t length, typename type_t>
        Number(const Number<length, type_t> &number) : m_data{}
        {
            memcpy(this->data(), number.data(), BITS <= length ? size() : number.size());
        }


        template<class data_t>
        Number(const data_t &object) : m_data{}
        {
            assert(this->size()        >= sizeof(data_t));
            memcpy(this->data(), &object, sizeof(data_t));
        }


        template<class char_t = char>
        Number(const String<char_t> &string, const String<char_t> &format) : Number(decode(string, format))
        {
        }


       ~Number()
        {
            memset(this->data(), 0, this->size());
        }


        size_t
        bits() const
        {
            return BITS;
        }


        size_t
        bins() const
        {
            return BINS;
        }


        size_t
        size() const
        {
            return SIZE;
        }


        word_t*
        data()
        {
            return this->m_data;
        }


        const word_t*
        data() const
        {
            return this->m_data;
        }


        // ::sub


        word_t&
        operator[](const size_t &offset)
        {
            return this->m_data[offset];
        }


        const word_t&
        operator[](const size_t &offset) const
        {
            return this->m_data[offset];
        }


        // ::add


        Number&
        operator+=(const size_t &rvalue)
        {
            size_t remain = rvalue;

            for (size_t i = 0; i < BINS && remain; ++i)
            {
                m_data[i] = (word_t)(remain += m_data[i]);
                remain    = (size_t)(remain >> WORD_BIT);
            }

            return *this;
        }


        friend Number
        operator+(const Number &lvalue, const size_t &rvalue)
        {
            return Number(lvalue) += rvalue;
        }


        friend Number
        operator+(const size_t &lvalue , const Number &rvalue)
        {
            return Number(rvalue) += lvalue;
        }


        // ::mul


        Number&
        operator*=(const size_t &rvalue)
        {
            size_t remain = 0;

            for (size_t i = 0; i < BINS; ++i)
            {
        	    m_data[i] = (word_t)(remain += m_data[i] * rvalue);
	            remain    = (size_t)(remain >> WORD_BIT);
            }

            return *this;
        }


        friend Number
        operator*(const Number &lvalue, const size_t &rvalue)
        {
            return Number(lvalue) *= rvalue;
        }


        friend Number
        operator*(const size_t &lvalue , const Number &rvalue)
        {
            return Number(rvalue) *= lvalue;
        }


        // ::div


        size_t
        divide(const size_t &rvalue)
        {
            size_t remain = 0;

            for (int i = int(BINS - 1); i >= 0; --i)
            {
                if ((remain <<= WORD_BIT) += this->m_data[i])
                {
                    m_data[i] = (word_t)(remain / rvalue);
                    remain    = (size_t)(remain % rvalue);
                }
                else
                {
                    m_data[i] = 0;
                }
            }

            return remain;
        }


        Number&
        operator/=(const size_t &rvalue)
        {
            return (divide(rvalue), *this);
        }


        friend Number
        operator/(const Number &lvalue, const size_t &rvalue)
        {
            return Number(lvalue) /= rvalue;
        }


        // ::not


        bool
        operator!() const
        {
            return memcmp(this->data(), ZERO.data(), this->size()) == 0;
        }


        // ::eql


        friend bool
        operator==(const Number &lvalue, const Number &rvalue)
        {
            return memcmp(lvalue.data(), rvalue.data(), rvalue.size()) == 0;
        }


        // ::neq


        friend bool
        operator!=(const Number &lvalue, const Number &rvalue)
        {
            return memcmp(lvalue.data(), rvalue.data(), rvalue.size()) != 0;
        }


        // ::encode


        template<class char_t = char> String<char_t>
        encode(const String<char_t> &format = BASE16) const
        {
            String<char_t>  string;
            Number          number(*this);
            size_t digits = format.size();

            string.reserve(size_t(this->bits() / log2(digits)));

            while (!!number)
            {
                string.push_back(format[number.divide(digits)]);
            }

            if (!string.size()) string.push_back(format[0]);
            return String<>(string.rbegin(), string.rend());
        }


        // ::decode


        template<class char_t = char> static Number
        decode(const String<char_t> &string, const String<char_t> &format = BASE16)
        {
            Number number;

            for (const auto &lexeme : string)
            {
                size_t offset = format.find(lexeme);

                if (offset == String<char_t>::npos)
                {
                    return Number();
                }

                (number *= format.size()) += offset;
            }

            return number;
        }
    };


    // swap()


    uint8_t
    swap(const uint8_t &number)
    {
        return number;
    }


    uint16_t
    swap(const uint16_t &number)
    {
        #if defined(_WIN32)
            return _byteswap_ushort( number );
        #elif defined(__linux__)
            return __builtin_bswap16( number );
        #else
            #error "Architecture not supported."
        #endif
    }


    uint32_t
    swap(const uint32_t &number)
    {
        #if defined(_WIN32)
            return _byteswap_ulong( number );
        #elif defined(__linux__)
            return __builtin_bswap32( number );
        #else
            #error "Architecture not supported."
        #endif
    }


    uint64_t
    swap(const uint64_t &number)
    {
        #if defined(_WIN32)
            return _byteswap_uint64( number );
        #elif defined(__linux__)
            return __builtin_bswap64( number );
        #else
            #error "Architecture not supported."
        #endif
    }


    template<size_t BITS, typename word_t> Number<BITS, word_t>
    swap(const Number<BITS, word_t> &number)
    {
        Number<BITS, word_t> result;

        for (size_t i = 0; i < number.bins(); ++i)
        {
            result[i] = swap(number[number.bins() - i - 1]);
        }

        return result;
    }


    #if BYTE_ORDER == LITTLE_ENDIAN
        #define h2le(x) (x)
        #define h2be(x) swap(x)
        #define le2h(x) (x)
        #define be2h(x) swap(x)
    #elif BYTE_ORDER == BIG_ENDIAN
        #define h2le(x) swap(x)
        #define h2be(x) (x)
        #define le2h(x) swap(x)
        #define be2h(x) (x)
    #else
        #error "Architecture not supported."
    #endif


    #if SIZE_MAX == UINT32_MAX
        typedef Number<128, uint16_t> uint128_t;
        typedef Number<160, uint16_t> uint160_t;
        typedef Number<256, uint16_t> uint256_t;
        typedef Number<512, uint16_t> uint512_t;
    #elif SIZE_MAX == UINT64_MAX
        typedef Number<128, uint32_t> uint128_t;
        typedef Number<160, uint32_t> uint160_t;
        typedef Number<256, uint32_t> uint256_t;
        typedef Number<512, uint32_t> uint512_t;
    #else
        #error "Architecture not supported."
    #endif
}
