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
#include "../hasher.h"

namespace crypto
{
    template<size_t BITS> auto
    rmd(const void *record, const size_t &length)
    {
        return hasher::RMD<BITS>().update(record, length).digest();
    }


    template<size_t BITS, size_t length> auto
    rmd(const Number<length> &number)
    {
        return rmd<BITS>(number.data(), number.size());
    }


    template<size_t BITS, class char_t> auto
    rmd(const String<char_t> &string)
    {
        return rmd<BITS>(string.data(), string.size());
    }


    template<size_t BITS> auto
    rmd(const char *string)
    {
        return rmd<BITS>((void*)(string), strlen(string));
    }


    template<size_t BITS, class data_t> auto
    rmd(const data_t &object)
    {
        return rmd<BITS>((void*)&object, sizeof(data_t));
    }


    namespace hasher
    {
        template<size_t BITS>
        class RMD : public Hasher<BITS, BITS>
        {
            template<size_t> struct Option;

            template<> struct Option<160>
            {
                enum  : size_t
                {
                    STATES =  5,
                    BLOCKS = 16,
                    ROUNDS = 10,
                };

                typedef uint32_t word_t;
                typedef uint64_t long_t;
            };


            typedef typename Option<BITS>     option;
            typedef typename option::word_t   word_t;
            typedef typename option::long_t   long_t;

            enum  : size_t
            {
                WORD_BIT = sizeof(word_t) * CHAR_BIT,
                STATES   =            option::STATES,
                BLOCKS   =            option::BLOCKS,
                ROUNDS   =            option::ROUNDS,
            };

            Number<STATES * WORD_BIT, word_t> m_hash;
            Number<BLOCKS * WORD_BIT, byte_t> m_data;


        public:

            static const Number<STATES * WORD_BIT, word_t> SEED;
            static const Number<ROUNDS * WORD_BIT, word_t> SALT;
            static const int               OFFS[ROUNDS][BLOCKS];
            static const int               SIZE[ROUNDS][BLOCKS];


            RMD() : Hasher(), m_hash{RMD::SEED}, m_data{}
            {
            }


           ~RMD()
            {
            }


            const byte_t*
            hash() const
            {
                return (byte_t*)(this->m_hash.data());
            }


            byte_t*
            data()
            {
                return (byte_t*)(this->m_data.data());
            }


            const byte_t*
            data() const
            {
                return (byte_t*)(this->m_data.data());
            }


            size_t
            capacity() const
            {
                return (size_t)(this->m_data.size());
            }


        protected:


            void
            compress()
            {
                Number<STATES * WORD_BIT, word_t> lstate{m_hash};
                Number<STATES * WORD_BIT, word_t> rstate{m_hash};
                Number<BLOCKS * WORD_BIT, word_t> blocks{m_data};
                word_t buffer{};

                for (size_t i = 0; i < BLOCKS; ++i)
                {
                    blocks[i] = h2le(blocks[i]);
                }

                #define RMD160(s, salt, offs, size, oper) \
                {\
                    buffer = rotl(s[0] + oper(s[1], s[2], s[3]) + salt + blocks[offs], size);\
                    s[0]=s[4]; s[4]=s[3]; s[3]=rotl(s[2], 10); s[2]=s[1]; s[1]=s[0] + buffer;\
                }

                // loop on the rounds is slow due to boolean operators
                // the optimizer fails to inline an array of functions

                for (size_t i = 0, j = i + ROUNDS / 2, k = 0; k < BLOCKS; ++k)
                {
                    RMD160(lstate, SALT[i], OFFS[i][k], SIZE[i][k], boop150);
                    RMD160(rstate, SALT[j], OFFS[j][k], SIZE[j][k], boop045);
                }

                for (size_t i = 1, j = i + ROUNDS / 2, k = 0; k < BLOCKS; ++k)
                {
                    RMD160(lstate, SALT[i], OFFS[i][k], SIZE[i][k], boop202);
                    RMD160(rstate, SALT[j], OFFS[j][k], SIZE[j][k], boop228);
                }

                for (size_t i = 2, j = i + ROUNDS / 2, k = 0; k < BLOCKS; ++k)
                {
                    RMD160(lstate, SALT[i], OFFS[i][k], SIZE[i][k], boop089);
                    RMD160(rstate, SALT[j], OFFS[j][k], SIZE[j][k], boop089);
                }

                for (size_t i = 3, j = i + ROUNDS / 2, k = 0; k < BLOCKS; ++k)
                {
                    RMD160(lstate, SALT[i], OFFS[i][k], SIZE[i][k], boop228);
                    RMD160(rstate, SALT[j], OFFS[j][k], SIZE[j][k], boop202);
                }

                for (size_t i = 4, j = i + ROUNDS / 2, k = 0; k < BLOCKS; ++k)
                {
                    RMD160(lstate, SALT[i], OFFS[i][k], SIZE[i][k], boop045);
                    RMD160(rstate, SALT[j], OFFS[j][k], SIZE[j][k], boop150);
                }

                #undef RMD160

                buffer    = m_hash[1] + lstate[2] + rstate[3];
                m_hash[1] = m_hash[2] + lstate[3] + rstate[4];
                m_hash[2] = m_hash[3] + lstate[4] + rstate[0];
                m_hash[3] = m_hash[4] + lstate[0] + rstate[1];
                m_hash[4] = m_hash[0] + lstate[1] + rstate[2];
                m_hash[0] = buffer;
            }


            void
            finalize()
            {
                const size_t length  =  this->size();
                this->update(size_t(1), byte_t(0x80));

                if (this->reserve() < sizeof(long_t))
                {
                    this->update(this->reserve(), 0x0);
                }

                this->update(reserve() - sizeof(long_t), 0x0);
                this->update(h2le(long_t(length) * CHAR_BIT));

                for (size_t i = 0; i < STATES; ++i)
                {
                    this->m_hash[i] = le2h(this->m_hash[i]);
                }
            }
        };


        // RMD<160>

        template<>
        decltype(RMD<160>::SEED) RMD<160>::SEED =
        {
            0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0,
        };

        template<>
        decltype(RMD<160>::SALT) RMD<160>::SALT =
        {
            0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E,
            0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000,
        };

        template<>
        decltype(RMD<160>::OFFS) RMD<160>::OFFS =
        {
            {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
            {  7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8 },
            {  3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12 },
            {  1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2 },
            {  4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13 },
            {  5, 14,  7,  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12 },
            {  6, 11,  3,  7,  0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2 },
            { 15,  5,  1,  3,  7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13 },
            {  8,  6,  4,  1,  3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14 },
            { 12, 15, 10,  4,  1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11 },
        };

        template<>
        decltype(RMD<160>::SIZE) RMD<160>::SIZE =
        {
            { 11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8 },
            {  7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12 },
            { 11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5 },
            { 11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12 },
            {  9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6 },
            {  8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6 },
            {  9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11 },
            {  9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5 },
            { 15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8 },
            {  8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11 },
        };
    }
}
