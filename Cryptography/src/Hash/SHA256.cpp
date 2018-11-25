#include "Hash/SHA256.h"
#include <vector>

#pragma intrinsic(_byteswap_ulong)

namespace Cryptography
{
    namespace Hash
    {
        #define ROTL(x,n) ((x << n) | (x >> ((sizeof(WORD) * 8) - n)))
        #define ROTR(x,n) ((x >> n) | (x << ((sizeof(WORD) * 8) - n)))
        #define ADD_MOD(x,n) ((x + n) % 0x100000000)

        const uint32_t SHA256::SHA256_H[8] = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
        };
        const uint32_t SHA256::SHA256_K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        SHA256::SHA256()
        {
            memcpy(m_h, SHA256_H, 32);
        }


        std::array<uint8_t, SHA256::OUTPUT_SIZE> SHA256::Hash(const unsigned char* p_message, const uint64_t& p_size)
        {
            uint32_t k = (BLOCK_SIZE * 8) - (((p_size * 8) + 65) % (BLOCK_SIZE * 8));
            const uint32_t blockCount = static_cast<uint32_t>((p_size * 8) + 65 + k) / (BLOCK_SIZE * 8);
            int64_t bytesLeft = p_size;

            std::vector<SHA256_Block> blocks;
            blocks.resize(blockCount);

            for (uint32_t i = 0; i < blockCount; ++i)
            {
                if(bytesLeft >= 0)
                {
                    memcpy(blocks[i].words, p_message, (bytesLeft < BLOCK_SIZE) ? bytesLeft : BLOCK_SIZE);
                    if (bytesLeft < BLOCK_SIZE)
                    {
                        memset(reinterpret_cast<unsigned char*>(blocks[i].words) + bytesLeft, 0,BLOCK_SIZE - bytesLeft);
                        *(reinterpret_cast<unsigned char*>(blocks[i].words) + bytesLeft) = 0x80;
                        bytesLeft = 0;
                    }
                    bytesLeft -= BLOCK_SIZE;
                }

                if(i == blockCount -1)
                    blocks[i].words[WORDS_PER_BLOCK - 1] = _byteswap_ulong(static_cast<unsigned long>(p_size * 8));
                p_message += BLOCK_SIZE;

                blocks[i].words[0]  = _byteswap_ulong(blocks[i].words[0]);
                blocks[i].words[1]  = _byteswap_ulong(blocks[i].words[1]);
                blocks[i].words[2]  = _byteswap_ulong(blocks[i].words[2]);
                blocks[i].words[3]  = _byteswap_ulong(blocks[i].words[3]);
                blocks[i].words[4]  = _byteswap_ulong(blocks[i].words[4]);
                blocks[i].words[5]  = _byteswap_ulong(blocks[i].words[5]);
                blocks[i].words[6]  = _byteswap_ulong(blocks[i].words[6]);
                blocks[i].words[7]  = _byteswap_ulong(blocks[i].words[7]);
                blocks[i].words[8]  = _byteswap_ulong(blocks[i].words[8]);
                blocks[i].words[9]  = _byteswap_ulong(blocks[i].words[9]);
                blocks[i].words[10] = _byteswap_ulong(blocks[i].words[10]);
                blocks[i].words[11] = _byteswap_ulong(blocks[i].words[11]);
                blocks[i].words[12] = _byteswap_ulong(blocks[i].words[12]);
                blocks[i].words[13] = _byteswap_ulong(blocks[i].words[13]);
                blocks[i].words[14] = _byteswap_ulong(blocks[i].words[14]);
                blocks[i].words[15] = _byteswap_ulong(blocks[i].words[15]);

            }

            for (SHA256_Block& block : blocks)
            {
                CompressBlock(block);
            }

            m_h[0]  = _byteswap_ulong(m_h[0]);
            m_h[1]  = _byteswap_ulong(m_h[1]);
            m_h[2]  = _byteswap_ulong(m_h[2]);
            m_h[3]  = _byteswap_ulong(m_h[3]);
            m_h[4]  = _byteswap_ulong(m_h[4]);
            m_h[5]  = _byteswap_ulong(m_h[5]);
            m_h[6]  = _byteswap_ulong(m_h[6]);
            m_h[7]  = _byteswap_ulong(m_h[7]);

            uint8_t* charH = reinterpret_cast<uint8_t*>(m_h);
            return std::array<uint8_t,OUTPUT_SIZE> { charH[0],  charH[1],  charH[2],  charH[3],
                                                     charH[4],  charH[5],  charH[6],  charH[7],
                                                     charH[8],  charH[9],  charH[10], charH[11],
                                                     charH[12], charH[13], charH[14], charH[15],
                                                     charH[16], charH[17], charH[18], charH[19],
                                                     charH[20], charH[21], charH[22], charH[23],
                                                     charH[24], charH[25], charH[26], charH[27],
                                                     charH[28], charH[29], charH[30], charH[31] };
        }

        void SHA256::CompressBlock(SHA256_Block& p_block)
        {
            WORD w[64];
            memcpy(w, p_block.words, sizeof(WORD) * 16);

            for(int i = 16; i < 64; ++i)
            {
                WORD s0 = ROTR(w[i - 15], 7) ^ ROTR(w[i - 15], 18) ^ (w[i - 15] >> 3);
                WORD s1 = ROTR(w[i -2], 17) ^ ROTR(w[i - 2], 19) ^ (w[i - 2] >> 10);
                w[i] = ADD_MOD(w[i - 16], s0 + w[i - 7] + s1);
            }          
            
            uint32_t a = m_h[0];
            uint32_t b = m_h[1];
            uint32_t c = m_h[2];
            uint32_t d = m_h[3];
            uint32_t e = m_h[4];
            uint32_t f = m_h[5];
            uint32_t g = m_h[6];
            uint32_t h = m_h[7];

            for(int i =0; i < 64; ++i)
            {
                uint32_t S1 = ROTR(e, 6) ^ ROTR(e, 11) ^ ROTR(e, 25);
                uint32_t ch = (e & f) ^ ((~e) & g);
                uint32_t T1 = ADD_MOD(h, S1 + ch + SHA256_K[i] + w[i]);
                    
                uint32_t S0 = ROTR(a, 2) ^ ROTR(a, 13) ^ ROTR(a, 22);
                uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                uint32_t T2 = ADD_MOD(S0, maj);

                h = g;
                g = f;
                f = e;
                e = ADD_MOD(d, T1);
                d = c;
                c = b;
                b = a;
                a = ADD_MOD(T1, T2);
            }

            m_h[0] = ADD_MOD(m_h[0], a);
            m_h[1] = ADD_MOD(m_h[1], b);
            m_h[2] = ADD_MOD(m_h[2], c);
            m_h[3] = ADD_MOD(m_h[3], d);
            m_h[4] = ADD_MOD(m_h[4], e);
            m_h[5] = ADD_MOD(m_h[5], f);
            m_h[6] = ADD_MOD(m_h[6], g);
            m_h[7] = ADD_MOD(m_h[7], h);
        }
    }
}
