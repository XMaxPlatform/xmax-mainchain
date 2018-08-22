#include <time.h>
#include <string>
#include <limits>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "src/number.h"
#include "src/hasher/sha.h"
#include "src/hasher/rmd.h"

using namespace crypto;
typedef void(*test_t)();


#define CAST(expr) #expr
#define TEXT(expr) CAST(expr)


#define TEST(expr)\
{\
    if (!(expr))\
        throw std::logic_error(__FILE__ "(" TEXT(__LINE__) "): [FAIL] " #expr);\
}


#define TIME(expr, iter, time)\
{\
    clock_t tick = clock();\
    for (size_t i = 0; i < iter; ++i)  auto data = expr;\
    time = 1000 * double(clock() - tick)/CLOCKS_PER_SEC;\
}


#define PERF(name, iter, expr, refr)\
{\
    double sec1 = 0, sec2 = 0;\
    std::cout << "Performance " << name << ": ";\
    TIME(expr, iter, sec1); TIME(refr, iter, sec2);\
    std::cout << sec1 << "/" << sec2 << "ms\n";\
}


static test_t TESTS[] =
{
    []( /* Number */ )
    {
        String<> formats[] = { BASE02, BASE08, BASE10, BASE16, BASE58, BASE64 };

        for (size_t i = 0, j = 0; i < 10; ++i, j += size_t(rand()))
        {
            TEST((Number<128, uint8_t >(j).encode(BASE10)) == std::to_string(j));
            TEST((Number<128, uint16_t>(j).encode(BASE10)) == std::to_string(j));
            TEST((Number<128, uint32_t>(j).encode(BASE10)) == std::to_string(j));
        }

        for (const String<> &format: formats)
        {
            String<>  string;

            for (size_t i = 0; i < 2 * format.size(); ++i)
            {
                string += format[(i + 1) % format.size()];

                TEST((Number<1024, uint8_t >(string, format).encode(format)) == string);
                TEST((Number<1024, uint16_t>(string, format).encode(format)) == string);
            }
        }
    },


    []( /* hasher::SHA */ )
    {
        String<> string;

        auto sha224 = [](const String<> &string) -> Number<224>
        {
            Number<224> result;

            SHA256_CTX     handle; SHA224_Init(&handle);
            SHA224_Update(&handle, string.data(), string.size());
            return SHA224_Final(result.data(), &handle), result;
        };

        auto sha256 = [](const String<> &string) -> Number<256>
        {
            Number<256> result;

            SHA256_CTX     handle; SHA256_Init(&handle);
            SHA256_Update(&handle, string.data(), string.size());
            return SHA256_Final(result.data(), &handle), result;
        };

        auto sha384 = [](const String<> &string) -> Number<384>
        {
            Number<384> result;

            SHA512_CTX     handle; SHA384_Init(&handle);
            SHA384_Update(&handle, string.data(), string.size());
            return SHA384_Final(result.data(), &handle), result;
        };

        auto sha512 = [](const String<> &string) -> Number<512>
        {
            Number<512> result;

            SHA512_CTX     handle; SHA512_Init(&handle);
            SHA512_Update(&handle, string.data(), string.size());
            return SHA512_Final(result.data(), &handle), result;
        };

        string.reserve(2000);

        for (size_t i = 0; i < string.capacity(); ++i)
        {
            string += char(rand() % std::numeric_limits<char>::max());

            TEST((sha<256, 224>(string)) == sha224(string));
            TEST((sha<256, 256>(string)) == sha256(string));
            TEST((sha<512, 384>(string)) == sha384(string));
            TEST((sha<512, 512>(string)) == sha512(string));
        }

        PERF("SHA224", 10000, (sha<256, 224>(string)), sha224(string));
        PERF("SHA256", 10000, (sha<256, 256>(string)), sha256(string));
        PERF("SHA384", 10000, (sha<512, 384>(string)), sha384(string));
        PERF("SHA512", 10000, (sha<512, 512>(string)), sha512(string));
    },


    []( /* hasher::RMD */ )
    {
        String<> string;

        auto rmd160 = [](const String<> &string) -> Number<160>
        {
            Number<160> result;

            RIPEMD160_CTX     handle; RIPEMD160_Init(&handle);
            RIPEMD160_Update(&handle, string.data(), string.size());
            return RIPEMD160_Final(result.data(), &handle), result;
        };

        string.reserve(2000);

        for (size_t i = 0; i < string.capacity(); ++i)
        {
            string += char(rand() % std::numeric_limits<char>::max());

            TEST((rmd<160>(string)) == rmd160(string));
        }

        PERF("RMD160", 10000, (rmd<160>(string)), rmd160(string));
    },

};


int
main(int, char*[])
{
    try
    {
        for (const test_t &test: TESTS) test();
    }
    catch (std::exception &error)
    {
        std::cout << error.what() << std::endl;
        return -1;
    }

    return 0;
}
