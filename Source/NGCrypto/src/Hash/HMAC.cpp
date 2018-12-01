#include "NGCrypto/Hash/HMAC.h"
#include "NGCrypto/Hash/SHA256.h"
#include <vector>

namespace Cryptography
{
    namespace Hash
    {
        std::array<uint8_t,32> HMAC::HMAC_SHA256(const uint8_t* p_key, uint64_t p_keyLength, 
                                    const uint8_t* p_message, uint64_t p_messageLength)
        {
            uint8_t key[SHA256::BLOCK_SIZE] = {0};

            if(p_keyLength > SHA256::BLOCK_SIZE)
                memcpy(key, &(SHA256().Hash(p_key, p_keyLength)[0]), SHA256::OUTPUT_SIZE);
            else
                memcpy(key, p_key, static_cast<size_t>(p_keyLength));

            uint8_t iPadKey [SHA256::BLOCK_SIZE];
            uint8_t oPadKey [SHA256::BLOCK_SIZE];
            memset(iPadKey, 0x36, SHA256::BLOCK_SIZE);
            memset(oPadKey, 0x5c, SHA256::BLOCK_SIZE);
            
            for(uint8_t i = 0; i < 4; ++i)
            {
                iPadKey[i * 16 + 0]  ^= key[i * 16 + 0];
                iPadKey[i * 16 + 1]  ^= key[i * 16 + 1];
                iPadKey[i * 16 + 2]  ^= key[i * 16 + 2];
                iPadKey[i * 16 + 3]  ^= key[i * 16 + 3];
                iPadKey[i * 16 + 4]  ^= key[i * 16 + 4];
                iPadKey[i * 16 + 5]  ^= key[i * 16 + 5];
                iPadKey[i * 16 + 6]  ^= key[i * 16 + 6];
                iPadKey[i * 16 + 7]  ^= key[i * 16 + 7];            
                iPadKey[i * 16 + 8]  ^= key[i * 16 + 8];
                iPadKey[i * 16 + 9]  ^= key[i * 16 + 9];
                iPadKey[i * 16 + 10] ^= key[i * 16 + 10];
                iPadKey[i * 16 + 11] ^= key[i * 16 + 11];
                iPadKey[i * 16 + 12] ^= key[i * 16 + 12];
                iPadKey[i * 16 + 13] ^= key[i * 16 + 13];
                iPadKey[i * 16 + 14] ^= key[i * 16 + 14];
                iPadKey[i * 16 + 15] ^= key[i * 16 + 15];
                           
                oPadKey[i * 16 + 0]  ^= key[i * 16 + 0];
                oPadKey[i * 16 + 1]  ^= key[i * 16 + 1];
                oPadKey[i * 16 + 2]  ^= key[i * 16 + 2];
                oPadKey[i * 16 + 3]  ^= key[i * 16 + 3];
                oPadKey[i * 16 + 4]  ^= key[i * 16 + 4];
                oPadKey[i * 16 + 5]  ^= key[i * 16 + 5];
                oPadKey[i * 16 + 6]  ^= key[i * 16 + 6];
                oPadKey[i * 16 + 7]  ^= key[i * 16 + 7];
                oPadKey[i * 16 + 8]  ^= key[i * 16 + 8];
                oPadKey[i * 16 + 9]  ^= key[i * 16 + 9];
                oPadKey[i * 16 + 10] ^= key[i * 16 + 10];
                oPadKey[i * 16 + 11] ^= key[i * 16 + 11];
                oPadKey[i * 16 + 12] ^= key[i * 16 + 12];
                oPadKey[i * 16 + 13] ^= key[i * 16 + 13];
                oPadKey[i * 16 + 14] ^= key[i * 16 + 14];
                oPadKey[i * 16 + 15] ^= key[i * 16 + 15];
            }

            std::vector<uint8_t>sha;
            sha.resize(static_cast<size_t>(p_messageLength + SHA256::BLOCK_SIZE), 0);
            memcpy(&sha[0],                  iPadKey, SHA256::BLOCK_SIZE);
            memcpy(&sha[SHA256::BLOCK_SIZE], p_message, static_cast<size_t>(p_messageLength));

            auto shaIPad = SHA256().Hash(&sha[0], SHA256::BLOCK_SIZE + p_messageLength);
            uint8_t hmac[96];
            memcpy(hmac, oPadKey, SHA256::BLOCK_SIZE);
            memcpy(hmac + SHA256::BLOCK_SIZE, shaIPad.data(), SHA256::OUTPUT_SIZE);
            return SHA256().Hash(reinterpret_cast<unsigned char*>(hmac), SHA256::BLOCK_SIZE + SHA256::OUTPUT_SIZE);
        }
    }
}
