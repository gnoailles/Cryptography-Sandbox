#pragma once
#include "NGMP.h"
#include "NGCrypto/export.h"

#pragma warning(push)
#pragma warning(disable: 4251)

#define PUBLIC_KEY_SIZE 2048
#define PRIVATE_KEY_SIZE 256
namespace Cryptography
{
    namespace KeyExchange
    {
        class NG_CRYPTO_API DiffieHellman
        {
        private:

                //Using RFC3526 MODP Groups
        #if PUBLIC_KEY_SIZE == 2048
            static const uint32_t       PRIME_ARRAY[64];
            static const uint8_t        GENERATOR = 2;
            #define OVERFLOW_SAFE_SIZE 4096

        #elif PUBLIC_KEY_SIZE == 1536
            static const uint32_t       PRIME_ARRAY[48];
            static const uint8_t        GENERATOR = 2;
            #define OVERFLOW_SAFE_SIZE 2048

        #elif PUBLIC_KEY_SIZE == 1024
            static const uint32_t       PRIME_ARRAY[32];
            static const uint8_t        GENERATOR = 2;
            #define OVERFLOW_SAFE_SIZE 2048
        #endif

            static const NGMP<PUBLIC_KEY_SIZE>  PRIME;

        public:
            DiffieHellman() = delete;
            ~DiffieHellman() = delete;

            static void                  GenerateKeyPair(        NGMP<PRIVATE_KEY_SIZE>& p_privateKey,        NGMP<PUBLIC_KEY_SIZE>&  p_publicKey);
            static NGMP<PUBLIC_KEY_SIZE> GenerateSharedKey(const NGMP<PUBLIC_KEY_SIZE>&  p_otherPublic, const NGMP<PRIVATE_KEY_SIZE>& p_privateKey);
        };
    }
}

#pragma warning(pop)