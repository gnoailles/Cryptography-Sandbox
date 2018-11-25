#pragma once
#include <cstdint>
#include <array>

namespace Cryptography
{
    namespace Hash
    {
        class SHA256
        {
            using WORD = uint32_t;

        public:
            static const uint8_t BLOCK_SIZE = 64;
            static const uint8_t OUTPUT_SIZE = 32;
            static const uint8_t WORDS_PER_BLOCK = {BLOCK_SIZE / (sizeof(WORD))};

        private:
            struct SHA256_Block
            {
                WORD words[WORDS_PER_BLOCK] = {0};
            };

            static const uint32_t SHA256_H[8];
            static const uint32_t SHA256_K[64];

            uint32_t m_h[8] {0};

            void CompressBlock(SHA256_Block& p_block);
        public:
            SHA256();
            ~SHA256() = default;

            std::array<uint8_t,OUTPUT_SIZE> Hash(const unsigned char* p_message, const uint64_t& p_size);
        };
    }
}
