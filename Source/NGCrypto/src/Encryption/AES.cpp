#include "NGCrypto/Encryption/AES.h"
#include <cassert>
#include <Windows.h>

namespace Cryptography
{ 
    namespace Encryption
    {
        void AES::KeyAssist1(__m128i* p_temp1, __m128i* p_temp2)
        {
            *p_temp2 = _mm_shuffle_epi32(*p_temp2, 0xff);

            __m128i temp4 = _mm_slli_si128(*p_temp1, 0x4);
            *p_temp1 = _mm_xor_si128(*p_temp1, temp4);        
            temp4 = _mm_slli_si128(*p_temp1, 0x4);
            *p_temp1 = _mm_xor_si128(*p_temp1, temp4);
            temp4 = _mm_slli_si128(*p_temp1, 0x4);
            *p_temp1 = _mm_xor_si128(*p_temp1, temp4);

            *p_temp1 = _mm_xor_si128(*p_temp1, *p_temp2);
        }

        void AES::KeyAssist2(__m128i* p_temp1, __m128i* p_temp3)
        {
            __m128i temp4 = _mm_aeskeygenassist_si128(*p_temp1, 0x0);
            const __m128i temp2 = _mm_shuffle_epi32(temp4, 0xaa);

            temp4 = _mm_slli_si128(*p_temp3, 0x4);
            *p_temp3 = _mm_xor_si128(*p_temp3, temp4);
            temp4 = _mm_slli_si128(*p_temp3, 0x4);
            *p_temp3 = _mm_xor_si128(*p_temp3, temp4);
            temp4 = _mm_slli_si128(*p_temp3, 0x4);
            *p_temp3 = _mm_xor_si128(*p_temp3, temp4);

            *p_temp3 = _mm_xor_si128(*p_temp3, temp2);

        }

        void AES::GenerateEncryptionRoundKeys()
        {

            __m128i temp1 = cipherKey[0];
            __m128i temp3 = cipherKey[1];

            encryptionRoundKeys[0] = temp1;
            encryptionRoundKeys[1] = temp3;

            __m128i temp2 = _mm_aeskeygenassist_si128(temp3, 0x01);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[2] = temp1;
            KeyAssist2(&temp1, &temp3);
            encryptionRoundKeys[3] = temp3;

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x02);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[4] = temp1;
            KeyAssist2(&temp1, &temp3);
            encryptionRoundKeys[5] = temp3;
            
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x04);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[6] = temp1;
            KeyAssist2(&temp1, &temp3);
            encryptionRoundKeys[7] = temp3;
            
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x08);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[8] = temp1;
            KeyAssist2(&temp1, &temp3);
            encryptionRoundKeys[9] = temp3;
            
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x10);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[10] = temp1;
            KeyAssist2(&temp1, &temp3);
            encryptionRoundKeys[11] = temp3;

            
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x20);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[12] = temp1;
            KeyAssist2(&temp1, &temp3);
            encryptionRoundKeys[13] = temp3;

            
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x40);
            KeyAssist1(&temp1, &temp2);
            encryptionRoundKeys[14] = temp1;
        }

        void AES::GenerateDecryptionRoundKeys()
        {
            decryptionRoundKeys[0] = encryptionRoundKeys[ROUND_COUNT];
            for (int i = 1; i < ROUND_COUNT; ++i)
            {
                decryptionRoundKeys[i] = _mm_aesimc_si128(encryptionRoundKeys[ROUND_COUNT - i]);
            }
            decryptionRoundKeys[ROUND_COUNT] = encryptionRoundKeys[0];
        }

        AES::AES(const unsigned char p_cipherKey[64])
        {
            assert(IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE));

            cipherKey[0] = _mm_loadu_si128(reinterpret_cast<const __m128i*>(p_cipherKey));
            cipherKey[1] = _mm_loadu_si128(reinterpret_cast<const __m128i*>(p_cipherKey + 16));

            GenerateEncryptionRoundKeys();
            GenerateDecryptionRoundKeys();

        }

        void AES::EncryptECB(const unsigned char* p_data, unsigned char* p_out, uint64_t p_dataLength)
        {
            if(p_dataLength % 16) 
                p_dataLength = p_dataLength / 16 + 1; 
            else 
                p_dataLength = p_dataLength / 16;

            for(uint32_t i = 0; i < p_dataLength; ++i)
            {
                __m128i temp = _mm_loadu_si128(&(reinterpret_cast<const __m128i*>(p_data))[i]);
                temp = _mm_xor_si128(temp, encryptionRoundKeys[0]);

                for(int j = 1; j < ROUND_COUNT; j++)
                {
                    temp = _mm_aesenc_si128(temp, encryptionRoundKeys[j]);
                }

                temp = _mm_aesenclast_si128(temp, encryptionRoundKeys[ROUND_COUNT]);

                _mm_storeu_si128(&(reinterpret_cast<__m128i*>(p_out))[i], temp);
            }
        }

        void AES::DecryptECB(const unsigned char* p_data, unsigned char* p_out, uint64_t p_dataLength)
        {
            if(p_dataLength % 16) 
                p_dataLength = p_dataLength / 16 + 1; 
            else 
                p_dataLength = p_dataLength / 16;

            for(uint32_t i = 0; i < p_dataLength; ++i)
            {
                __m128i temp = _mm_loadu_si128(&(reinterpret_cast<const __m128i*>(p_data))[i]);
                temp = _mm_xor_si128(temp, decryptionRoundKeys[0]);

                for(int j = 1; j < ROUND_COUNT; j++)
                {
                    temp = _mm_aesdec_si128(temp, decryptionRoundKeys[j]);
                }

                temp = _mm_aesdeclast_si128(temp, decryptionRoundKeys[ROUND_COUNT]);

                _mm_storeu_si128(&(reinterpret_cast<__m128i*>(p_out))[i], temp);
            }
        }

        void AES::EncryptCBC(const unsigned char* p_data, unsigned char* p_out, uint64_t p_dataLength)
        {
        }

        void AES::DecryptCBC(const unsigned char* p_data, unsigned char* p_out, uint64_t p_dataLength)
        {
        }
    }
}
