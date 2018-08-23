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
    template<size_t BITS, size_t VITS = BITS> auto
    sha(const void *record, const size_t &length)
    {
        return hasher::SHA<BITS, VITS>().update(record, length).digest();
    }


    template<size_t BITS, size_t VITS = BITS, size_t length> auto
    sha(const Number<length> &number)
    {
        return sha<BITS, VITS>(number.data(), number.size());
    }


    template<size_t BITS, size_t VITS = BITS, class char_t> auto
    sha(const String<char_t> &string)
    {
        return sha<BITS, VITS>(string.data(), string.size());
    }


    template<size_t BITS, size_t VITS = BITS> auto
    sha(const char *string)
    {
        return sha<BITS, VITS>((void*)(string), strlen(string));
    }


    template<size_t BITS, size_t VITS = BITS, class data_t> auto
    sha(const data_t &object)
    {
        return sha<BITS, VITS>((void*)&object, sizeof(data_t));
    }


    namespace hasher
    {
        template<size_t BITS, size_t VITS = BITS>
        class SHA : public Hasher<BITS, VITS>
        {
            template<size_t> struct Option;

            template<> struct Option<256>
            {
                enum  : size_t
                {
                    STATES = 16,
                    BLOCKS = 16,
                    ROUNDS = 64,
                };

                typedef uint32_t  word_t;
                typedef uint64_t  long_t;
            };

            template<> struct Option<512>
            {
                enum  : size_t
                {
                    STATES = 16,
                    BLOCKS = 16,
                    ROUNDS = 80,
                };

                typedef uint64_t  word_t;
                typedef uint128_t long_t;
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


            SHA() : Hasher(), m_hash{SHA::SEED}, m_data{}
            {
            }


           ~SHA()
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
                Number<STATES * WORD_BIT, word_t> states{m_hash};
                Number<ROUNDS * WORD_BIT, word_t> rounds{m_data};

                for (size_t i = 0; i < BLOCKS; ++i)
                {
                    rounds[i] = h2be(rounds[i]);
                }

                for (size_t i = BLOCKS; i < ROUNDS; ++i)
                {
                    rounds[i] = rounds[i-16] + sigma0(rounds[i-15]) + rounds[i-7] + sigma1(rounds[i-2]);
                }

                for (size_t i = 0; i < ROUNDS; ++i)
                {
                    word_t t0 = delta0(states[0]) + boop232(states[0], states[1], states[2]);
                    word_t t1 = delta1(states[4]) + boop202(states[4], states[5], states[6])
                              + states[7] + rounds[i] + SHA<BITS,BITS>::SALT[i];

                    states[7] = states[6];
                    states[6] = states[5];
                    states[5] = states[4];
                    states[4] = states[3] + t1;
                    states[3] = states[2];
                    states[2] = states[1];
                    states[1] = states[0];
                    states[0] = t1 + t0;
                }

                for (size_t i = 0; i < STATES; ++i)
                {
                    this->m_hash[i] += states[i];
                }
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
                this->update(h2be(long_t(length) * CHAR_BIT));

                for (size_t i = 0; i < STATES; ++i)
                {
                    this->m_hash[i] = be2h(this->m_hash[i]);
                }
            }


            static uint32_t
            sigma0(const uint32_t &number)
            {
                return rotr(number, 7) ^ rotr(number,18) ^ (number >>  3);
            }


            static uint64_t
            sigma0(const uint64_t &number)
            {
                return rotr(number, 1) ^ rotr(number, 8) ^ (number >>  7);
            }


            static uint32_t
            sigma1(const uint32_t &number)
            {
                return rotr(number,17) ^ rotr(number,19) ^ (number >> 10);
            }


            static uint64_t
            sigma1(const uint64_t &number)
            {
                return rotr(number,19) ^ rotr(number,61) ^ (number >>  6);
            }


            static uint32_t
            delta0(const uint32_t &number)
            {
                return rotr(number, 2) ^ rotr(number,13) ^ rotr(number,22);
            }


            static uint64_t
            delta0(const uint64_t &number)
            {
                return rotr(number,28) ^ rotr(number,34) ^ rotr(number,39);
            }


            static uint32_t
            delta1(const uint32_t &number)
            {
                return rotr(number, 6) ^ rotr(number,11) ^ rotr(number,25);
            }


            static uint64_t
            delta1(const uint64_t &number)
            {
                return rotr(number,14) ^ rotr(number,18) ^ rotr(number,41);
            }
        };


        // SHA<256>


        template<>
        decltype(SHA<256, 224>::SEED) SHA<256, 224>::SEED =
        {
            0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939,
            0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4,
        };

        template<>
        decltype(SHA<256, 256>::SEED) SHA<256, 256>::SEED =
        {
            0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
            0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19,
        };

        template<>
        decltype(SHA<256, 256>::SALT) SHA<256, 256>::SALT =
        {
            0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
            0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
            0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
            0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
            0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
            0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
            0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
            0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
            0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
            0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
            0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
            0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
            0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
            0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
            0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
            0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
        };


        // SHA<512>

        template<>
        decltype(SHA<512, 224>::SEED) SHA<512, 224>::SEED =
        {
            0x8C3D37C819544DA2, 0x73E1996689DCD4D6, 0x1DFAB7AE32FF9C82, 0x679DD514582F9FCF,
            0x0F6D2B697BD44DA8, 0x77E36F7304C48942, 0x3F9D85A86A1D36C8, 0x1112E6AD91D692A1,
        };

        template<>
        decltype(SHA<512, 256>::SEED) SHA<512, 256>::SEED =
        {
            0x22312194FC2BF72C, 0x9F555FA3C84C64C2, 0x2393B86B6F53B151, 0x963877195940EABD,
            0x96283EE2A88EFFE3, 0xBE5E1E2553863992, 0x2B0199FC2C85B8AA, 0x0EB72DDC81C52CA2,
        };

        template<>
        decltype(SHA<512, 384>::SEED) SHA<512, 384>::SEED =
        {
            0xCBBB9D5DC1059ED8, 0x629A292A367CD507, 0x9159015A3070DD17, 0x152FECD8F70E5939,
            0x67332667FFC00B31, 0x8EB44A8768581511, 0xDB0C2E0D64F98FA7, 0x47B5481DBEFA4FA4,
        };

        template<>
        decltype(SHA<512, 512>::SEED) SHA<512, 512>::SEED =
        {
            0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
            0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179,
        };

        template<>
        decltype(SHA<512, 512>::SALT) SHA<512, 512>::SALT =
        {
            0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
            0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
            0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
            0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
            0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
            0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
            0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
            0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
            0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
            0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
            0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
            0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
            0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
            0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
            0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
            0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
            0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
            0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
            0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
            0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817,
        };
    }
}
