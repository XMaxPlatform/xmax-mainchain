# Crypto

Small, fast, header only, zero dependency cryptographic library written in C++17.

## Primitives

### Number

Implements a binned unsigned integer of fixed width. For example:

```C++
#include <crypto/number.h>
using namespace crypto;

typedef Number<256, uint8_t>  uint256_t; // 256 bits divided into 32 uint8_t bins
typedef Number<256, uint16_t> uint256_t; // 256 bits divided into 16 uint16_t bins
typedef Number<256, uint32_t> uint256_t; // 256 bits divided into 8 uint32_t bins
```

### String

An alias for std::basic_string. The following holds:

```C++
String<char>    string; // is std::string
String<wchar_t> string; // is std::wstring
String<>        string; // is std::string
```

## Hashers

### SHA

Implements the SHA-2 family of cryptographic hash functions designed by the US National Security Agency. Supports the following variants: SHA256/224, SHA256, SHA512/224, SHA512/256, SHA512/384, SHA512.

```C++
#include <crypto/hasher/sha.h>
using namespace crypto;

// hash strings

Number<224> digest = sha<256, 224>("Hello World!");
Number<256> digest = sha<256     >("Hello World!");
Number<384> digest = sha<512, 384>("Hello World!");
Number<512> digest = sha<512     >("Hello World!");

// hash structures

SomeStruct  object{0, 1, 200};
Number<512> digest = sha<512>(object);

// hash iteratively

hasher::SHA<512, 384> hasher;

for (size_t i = 0; i < l; ++i)
    hasher.update(object[i]);

Number<384> digest = hasher.digest();
```

This algorithm is verified and benchmarked against OpenSSL implementation of SHA-2.

### RIPEMD

Implements the RIPEMD family of cryptographic hash functions developed by the COSIC research group at the Katholieke Universiteit Leuven. Supports one variant: RMD160.


```C++
#include <crypto/hasher/rmd.h>
using namespace crypto;

// hash strings

Number<160> digest = rmd<160>("Hello World!");

// hash structures

SomeStruct  object{0, 1, 200};
Number<160> digest = rmd<160>(object);

// hash iteratively

hasher::RMD<160> hasher;

for (size_t i = 0; i < l; ++i)
    hasher.update(object[i]);

Number<160> digest = hasher.digest();
```

This algorithm is verified and benchmarked against OpenSSL implementation of RIPEMD.

## Installation

Download the sources to the folder of choice and include the desired headers.

## Contributing

Feature requests and bug reports are much appreciated!

## License

The Crypto library is licensed under the terms of the [MIT](https://choosealicense.com/licenses/mit/) license and is available for free.