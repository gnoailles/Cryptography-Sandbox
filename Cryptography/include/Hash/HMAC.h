#pragma once
#include <array>

namespace Cryptography
{
    namespace Hash
    {
        class HMAC
        {
        public:
            HMAC() = delete;
            ~HMAC() = delete;
            static std::array<uint8_t,32> HMAC_SHA256(const unsigned char* p_key, uint64_t p_keyLength,
                                             const unsigned char* p_message, uint64_t p_messageLength);
        };
    }
}
