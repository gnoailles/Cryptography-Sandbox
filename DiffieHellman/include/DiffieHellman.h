#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "BigUInt/BigUInt.h"

#define PUBLIC_KEY_SIZE 2048
#define PRIVATE_KEY_SIZE 256

class DiffieHellman
{
private:

    //Using RFC3526 MODP Groups
#if     PUBLIC_KEY_SIZE == 2048
        static const uint32_t       PRIME_ARRAY[64];
        static const uint8_t        GENERATOR = 2;
        #define OVERFLOW_SAFE_SIZE 4096
#elif   PUBLIC_KEY_SIZE == 1536
        static const uint32_t       PRIME_ARRAY[48];
        static const uint8_t        GENERATOR = 2;
        #define OVERFLOW_SAFE_SIZE 2048
#elif   PUBLIC_KEY_SIZE == 1024
        static const uint32_t       PRIME_ARRAY[32];
        static const uint8_t        GENERATOR = 2;
        #define OVERFLOW_SAFE_SIZE 2048
#endif
    static const BigUInt<PUBLIC_KEY_SIZE>  PRIME;

public:
    DiffieHellman() = delete;
    ~DiffieHellman() = delete;

    static void                     GenerateKeyPair(        BigUInt<PRIVATE_KEY_SIZE>& p_privateKey,        BigUInt<PUBLIC_KEY_SIZE>&  p_publicKey);
    static BigUInt<PUBLIC_KEY_SIZE> GenerateSharedKey(const BigUInt<PUBLIC_KEY_SIZE>&  p_otherPublic, const BigUInt<PRIVATE_KEY_SIZE>& p_privateKey);
};

#pragma warning(pop)