#pragma once
#include <array>
#include "NGCrypto/export.h"

namespace Cryptography
{
    namespace Hash
    {
        class NG_CRYPTO_API HMAC
        {
        public:
            static const uint32_t SIZE {32};
            HMAC() = delete;
            ~HMAC() = delete;
            static std::array<uint8_t, SIZE> HMAC_SHA256(const unsigned char* p_key, uint64_t p_keyLength,
                                             const unsigned char* p_message, uint64_t p_messageLength);
        };
    }
}
