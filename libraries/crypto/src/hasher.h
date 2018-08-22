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
#include "number.h"
#include "string.h"

namespace crypto
{
    template<size_t BITS, size_t VITS = BITS>
    class Hasher
    {
        size_t          m_tail;
        size_t          m_size;


    protected:

        typedef uint8_t byte_t;


    public:

        Hasher() : m_tail{ 0 }, m_size{ 0 }
        {
        }


        virtual
       ~Hasher()
        {
            this->m_tail = this->m_size = 0;
        }


        virtual const byte_t*
        hash() const
        {
            return nullptr;
        }


        const size_t&
        size() const
        {
            return this->m_size;
        }


        virtual byte_t*
        data()
        {
            return nullptr;
        }


        virtual const byte_t*
        data() const
        {
            return nullptr;
        }


        byte_t*
        begin()
        {
            return this->data();
        }


        const byte_t*
        begin() const
        {
            return this->data();
        }


        byte_t*
        end()
        {
            return this->data() + this->m_tail;
        }


        const byte_t*
        end() const
        {
            return this->data() + this->m_tail;
        }


        virtual size_t
        capacity() const
        {
            return 0;
        }


        size_t
        reserve() const
        {
            return this->capacity() - this->m_tail;
        }


        template<size_t length> Hasher&
        update(const Number<length> &number)
        {
            return update(number.data(), number.size());
        }


        template<class char_t> Hasher&
        update(const String<char_t> &string)
        {
            return update(string.data(), string.size());
        }


        Hasher&
        update(const char *record)
        {
            return update((void*)record, strlen(record));
        }


        template<class data_t> Hasher&
        update(const data_t &object)
        {
            return update((byte_t*)&object, sizeof(object));
        }


        Hasher&
        update(const void *record, const size_t &length)
        {
            size_t  volume , remain = length;
            byte_t *memory = (byte_t*)record;

            assert(this->size() < SIZE_MAX);

            while ((volume = this->reserve()) <= remain)
            {
                memcpy(this->end(), memory, volume);
                this->m_tail = (this->compress(), 0);
                memory += volume; remain -= volume;
            }

            memcpy(this->end(), memory, remain);
            m_tail += remain; m_size += length;
            return *this;
        }


        Hasher&
        update(const size_t &length, const byte_t &record)
        {
            size_t  volume, remain = length;

            assert(this->size() < SIZE_MAX);

            while ((volume = this->reserve()) <= remain)
            {
                memset(this->end(), record, volume);
                this->m_tail = (this->compress(), 0);
                remain -= volume;
            }

            memset(this->end(), record, remain);
            m_tail += remain; m_size += length;
            return *this;
        }


        Number<VITS>
        digest()
        {
            if (this->size() < SIZE_MAX)
            {
                this->finalize();
                this->m_size = SIZE_MAX;
            }

            return Number<VITS>(hash());
        }


    protected:


        virtual void
        compress()
        {
        }


        virtual void
        finalize()
        {
        }
    };


    // rotl()


    uint32_t
    rotl(const uint32_t &number, const int &length)
    {
        #if defined(_WIN32)
            return _lrotl(number, length);
        #else
            return (number << length) | (number >> (sizeof(number) * CHAR_BIT - length));
        #endif
    }


    uint64_t
    rotl(const uint64_t &number, const int &length)
    {
        #if defined(_WIN32)
            return _rotl64(number, length);
        #else
            return (number << length) | (number >> (sizeof(number) * CHAR_BIT - length));
        #endif
    }


    // rotr()


    uint32_t
    rotr(const uint32_t &number, const int &length)
    {
        #if defined(_WIN32)
            return _lrotr(number, length);
        #else
            return (number >> length) | (number << (sizeof(number) * CHAR_BIT - length));
        #endif
    }


    uint64_t
    rotr(const uint64_t &number, const int &length)
    {
        #if defined(_WIN32)
            return _rotr64(number, length);
        #else
            return (number >> length) | (number << (sizeof(number) * CHAR_BIT - length));
        #endif
    }


    // boop()


    template<typename uint_t> uint_t
    boop045(const uint_t &x, const uint_t &y, const uint_t &z) { return x ^ (y | ~z); }

    template<typename uint_t> uint_t
    boop089(const uint_t &x, const uint_t &y, const uint_t &z) { return (x | ~y) ^ z; }

    template<typename uint_t> uint_t
    boop150(const uint_t &x, const uint_t &y, const uint_t &z) { return x ^ y ^ z; }

    template<typename uint_t> uint_t
    boop202(const uint_t &x, const uint_t &y, const uint_t &z) { return (x & (y ^ z)) ^ z; }

    template<typename uint_t> uint_t
    boop228(const uint_t &x, const uint_t &y, const uint_t &z) { return (x & z) | (y & ~z); }

    template<typename uint_t> uint_t
    boop232(const uint_t &x, const uint_t &y, const uint_t &z) { return (x & y) | ((x ^ y) & z); }
}
