#include <iostream>
#include <iomanip>

#include "Utils/BigUInt/BigUInt.h"
#include "KeyExchange/DiffieHellman.h"
#include "Hash/SHA256.h"
#include "Hash/HMAC.h"
#include "Encryption/AES.h"

using namespace Cryptography;

void PrintByteArray(const unsigned char* p_array, uint32_t p_size);
void DiffieHellmanTest();
void SHA256_TestVectors();
void HMAC_SHA256_TestVectors();
void AES256_ECB_TestVectors();
void CombinedUsageExample();

int main()
{
    CombinedUsageExample();

    // DiffieHellmanTest();
    // SHA256_TestVectors();
    // HMAC_SHA256_TestVectors();
    // AES256_ECB_TestVectors();

    std::cin.get();
}

void PrintByteArray(const unsigned char* p_array, uint32_t p_size)
{
    for (uint32_t i = 0; i < p_size; ++i)
    {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << +p_array[i];
        if((i + 1) % 4 == 0)
            std::cout << ' ';
    }
     std::cout << '\n';
}

void DiffieHellmanTest()
{
    using namespace KeyExchange;
    std::cout << "\n\n===== Diffie Hellman Key Exchange =====\n\n";
    BigUInt<PRIVATE_KEY_SIZE>   private1;
    BigUInt<PUBLIC_KEY_SIZE>    public1;

    BigUInt<PRIVATE_KEY_SIZE>   private2;
    BigUInt<PUBLIC_KEY_SIZE>    public2;

    DiffieHellman::GenerateKeyPair(private1, public1);
    std::cout << "Client1 Private Key: \n" << private1 << "\n\n";
    std::cout << "Client1 Public  Key: \n" << public1 << "\n\n";

    std::cout << '\n';

    DiffieHellman::GenerateKeyPair(private2, public2);
    std::cout << "Client2 Private Key: \n" << private2 << "\n\n";
    std::cout << "Client2 Public  Key: \n" << public2 << "\n\n";

    std::cout << '\n';

    const auto shared1 = DiffieHellman::GenerateSharedKey(public2, private1);
    std::cout << "Client1 Shared  Key: \n" << shared1 << "\n\n";
    const auto shared2 = DiffieHellman::GenerateSharedKey(public1, private2);
    std::cout << "Client2 Shared  Key: \n" << shared2 << "\n\n";
}

// Test Vectors from NIST
void SHA256_TestVectors()
{
    using namespace Hash;

    const unsigned char* input;
    std::array<uint8_t, SHA256::OUTPUT_SIZE> output{};

    std::cout << "\n\n===== SHA 256 =====\n\n";
    std::cout << "Test Vectors:\n\n";

    std::cout << "Test 1:\n\n";
    {
        std::cout << "\tInput :\n";
        input = reinterpret_cast<const unsigned char*>("abc");
        std::cout << "\t" << input << "\n\n";
        std::cout << "\tExpected Output :\n";
        std::cout << "\tba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad\n\n";

        output = SHA256().Hash(input, 3);
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 2:\n\n";
    {
        std::cout << "\tInput :\n";
        input = reinterpret_cast<const unsigned char*>("");
        std::cout << "\t \"\" (empty string)\n\n";
        std::cout << "\tExpected Output :\n";
        std::cout << "\te3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855\n\n";

        output = SHA256().Hash(input, 0);
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 3:\n\n";
    {
        std::cout << "\tInput :\n";
        input = reinterpret_cast<const unsigned char*>("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
        std::cout << "\t" << input << " (56 bytes)\n\n";
        std::cout << "\tExpected Output :\n";
        std::cout << "\t248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1\n\n";

        output = SHA256().Hash(input, 56);
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 4:\n\n";
    {
        std::cout << "\tInput :\n";
        input = reinterpret_cast<const unsigned char*>("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
        std::cout << "\t" << input << " (112 bytes)\n\n";
        std::cout << "\tExpected Output :\n";
        std::cout << "\tcf5b16a7 78af8380 036ce59e 7b049237 0b249b11 e8f07a51 afac4503 7afee9d1\n\n";

        output = SHA256().Hash(input, 112);
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 5:\n\n";
    {
        std::cout << "\tInput :\n";
        input = new unsigned char[1000000];
        memset((void*)input, 'a', 1000000);

        std::cout << "\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.... (1,000,000 bytes)\n\n";
        std::cout << "\tExpected Output :\n";
        std::cout << "\tcdc76e5c 9914fb92 81a1c7e2 84d73e67 f1809a48 a497200e 046d39cc c7112cd0\n\n";

        output = SHA256().Hash(input, 1000000);
        delete[] input;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }
    
    
}

// Test Vectors from RFC4231
void HMAC_SHA256_TestVectors()
{
    using namespace Hash;

    const unsigned char* key;
    const unsigned char* data;
    std::array<uint8_t, SHA256::OUTPUT_SIZE> output{};

    std::cout << "\n\n===== HMAC - SHA 256 =====\n\n";
    std::cout << "Test Vectors:\n\n";

    std::cout << "Test 1:\n\n";
    {
        std::cout << "\tInputs :\n";
        key = new const unsigned char[20]{0x0b};
        memset((void*)key, 0x0b, 20);

        data = reinterpret_cast<const unsigned char*>("Hi There");
        std::cout << "\t\t Key : 0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b (20 bytes)\n\n";
        std::cout << "\t\t Data : \"" << data << "\"\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\tb0344c61 d8db3853 5ca8afce af0bf12b 881dc200 c9833da7 26e9376c 2e32cff7\n\n";

        output = HMAC::HMAC_SHA256(key, 20, data, 8);
        delete[] key;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }
    
    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 2:\n\n";
    {
        std::cout << "\tInputs :\n";

        key = reinterpret_cast<const unsigned char*>("Jefe");
        data = reinterpret_cast<const unsigned char*>("what do ya want for nothing?");
        std::cout << "\t\t Key : \"" << key << "\"\n\n";
        std::cout << "\t\t Data : \"" << data << "\"\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t5bdcc146 bf60754e 6a042426 089575c7 5a003f08 9d273983 9dec58b9 64ec3843\n\n";

        output = HMAC::HMAC_SHA256(key, 4, data, 28);
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 3:\n\n";
    {
        std::cout << "\tInputs :\n";

        key = new const unsigned char[20]{0xaa};
        memset((void*)key, 0xaa, 20);
        data = new const unsigned char[50]{0xdd};
        memset((void*)data, 0xdd, 50);
        std::cout << "\t\t Key : 0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa (20 bytes)\n\n";
        std::cout << "\t\t Data : 0xdddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd (50 bytes)\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t773ea91e 36800e46 854db8eb d09181a7 2959098b 3ef8c122 d9635514 ced565fe\n\n";

        output = HMAC::HMAC_SHA256(key, 20, data, 50);
        delete[] key;
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 4:\n\n";
    {
        std::cout << "\tInputs :\n";

        key = new const unsigned char[25]
        {
            0x01, 0x02, 0x03, 0x04, 0x05,
            0x06, 0x07, 0x08, 0x09, 0x0a,
            0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14,
            0x15, 0x16, 0x17, 0x18, 0x19
        };
        data = new const unsigned char[50]{0xcd};
        memset((void*)data, 0xcd, 50);
        std::cout << "\t\t Key : 0x0102030405060708090a0b0c0d0e0f10111213141516171819 (25 bytes)\n\n";
        std::cout << "\t\t Data : 0xcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd (50 bytes)\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t82558a38 9a443c0e a4cc8198 99f2083a 85f0faa3 e578f807 7a2e3ff4 6729665b\n\n";

        output = HMAC::HMAC_SHA256(key, 25, data, 50);
        delete[] key;
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 5:\n\n";
    {
        std::cout << "\tInputs :\n";

        key = new const unsigned char[20]{};
        memset((void*)key, 0x0c, 20);
        data = reinterpret_cast<const unsigned char*>("Test With Truncation");
        std::cout << "\t\t Key : 0x0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c (20 bytes)\n\n";
        std::cout << "\t\t Data : \"" << data << "\"\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\ta3b61674 73100ee0 6e0c796c 2955552b\n\n";

        output = HMAC::HMAC_SHA256(key, 20, data, 20);
        delete[] key;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE / 2);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 6:\n\n";
    {
        std::cout << "\tInputs :\n";

        key = new const unsigned char[131]{};
        memset((void*)key, 0xaa, 131);
        data = reinterpret_cast<const unsigned char*>("Test Using Larger Than Block-Size Key - Hash Key First");
        std::cout << "\t\t Key : 0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa....aa (131 bytes)\n\n";
        std::cout << "\t\t Data : \"" << data << "\"\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t60e43159 1ee0b67f 0d8a26aa cbf5b77f 8e0bc621 3728c514 0546040f 0ee37f54\n\n";

        output = HMAC::HMAC_SHA256(key, 131, data, 54);
        delete[] key;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }

    std::cout << "\n\t------------------------------\n";

    std::cout << "\nTest 7:\n\n";
    {
        std::cout << "\tInputs :\n";

        key = new const unsigned char[131]{};
        memset((void*)key, 0xaa, 131);
        data = reinterpret_cast<const unsigned char*>("This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.");
        std::cout << "\t\t Key : 0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa....aa (131 bytes)\n\n";
        std::cout << "\t\t Data : \"" << data << "\"\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t9b09ffa7 1b942fcb 27635fbc d5b0e944 bfdc6364 4f071393 8a7f5153 5c3a35e2\n\n";

        output = HMAC::HMAC_SHA256(key, 131, data, 152);
        delete[] key;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
    }
}

// Tests from NIST SP 800-38A
void AES256_ECB_TestVectors()
{
    using namespace Encryption;

    const unsigned char* key;
    const unsigned char* data;
    std::vector<uint8_t> output{};

    std::cout << "\n\n===== AES 256 - ECB Mode =====\n\n";
    std::cout << "Test Vectors:\n\n";

    std::cout << "Test 1:\n\n";
    {
        std::cout << "\tInputs :\n";
        key = new const unsigned char[32] { 
            0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE,
            0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 
            0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7,
            0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 
        };
        std::cout << "\t\t Key : 603deb10 15ca71be 2b73aef0 857d7781 1f352c07 3b6108d7 2d9810a3 0914dff4\n\n";

        AES aes(key);
        delete[] key;

        data = new const unsigned char[64] {    
            0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
            0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
            0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C,
            0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
            0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11,
            0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF, 
            0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17,
            0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10 
        };
        std::cout << "\t\t Data : 6bc1bee2 2e409f96 e93d7e11 7393172a ae2d8a57 1e03ac9c 9eb76fac 45af8e51 30c81c46 a35ce411 e5fbc119 1a0a52ef f69f2445 df4f9b17 ad2b417b e66c3710\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\tf3eed1bd b5d2a03c 064b5a7e 3db181f8 591ccb10 d410ed26 dc5ba74a 31362870 b6ed21b9 9ca6f4f9 f153e7b1 beafed1d 23304b7a 39f9f3ff 067d8d8f 9e24ecc7 \n\n";
        
        output.reserve(64);
        aes.EncryptECB(data, output.data(), 64);
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), 64);

        aes.DecryptECB(output.data(), output.data(), 64);

        std::cout << "\n\n\tDecrypted Output :\n\t";
        PrintByteArray(output.data(), 64);
    }
    
    std::cout << "\n\t------------------------------\n";
    
    std::cout << "Test 2:\n\n";
    {
        std::cout << "\tInputs :\n";
        key = new const unsigned char[32] { 
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
            0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,      
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
            0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4   
        };
        std::cout << "\t\t Key : 603deb10 15ca71be 2b73aef0 857d7781 1f352c07 3b6108d7 2d9810a3 0914dff4\n\n";

        AES aes(key);
        delete[] key;

        data = new const unsigned char[16] {    
            0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
            0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
        };
        std::cout << "\t\t Data : 6bc1bee2 2e409f96 e93d7e11 7393172a\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\tf3eed1bd b5d2a03c 064b5a7e 3db181f8\n\n";
        
        output.resize(16);
        aes.EncryptECB(data, output.data(), 16);
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), 16);

        aes.DecryptECB(output.data(), output.data(), 16);

        std::cout << "\n\n\tDecrypted Output :\n\t";
        PrintByteArray(output.data(), 16);
    }
    
    std::cout << "\n\t------------------------------\n";
    
    std::cout << "Test 3:\n\n";
    {
        std::cout << "\tInputs :\n";
        key = new const unsigned char[32] { 
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
            0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,      
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
            0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4   
        };
        std::cout << "\t\t Key : 603deb10 15ca71be 2b73aef0 857d7781 1f352c07 3b6108d7 2d9810a3 0914dff4\n\n";

        AES aes(key);
        delete[] key;

        data = new const unsigned char[16] {    
           0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
           0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51 
        };

        std::cout << "\t\t Data : ae2d8a57 1e03ac9c 9eb76fac 45af8e51\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t591ccb10 d410ed26 dc5ba74a 31362870 \n\n";
        
        output.resize(16);
        aes.EncryptECB(data, output.data(), 16);
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), 16);

        aes.DecryptECB(output.data(), output.data(), 16);

        std::cout << "\n\n\tDecrypted Output :\n\t";
        PrintByteArray(output.data(), 16);
    }
    
    std::cout << "\n\t------------------------------\n";
    
    std::cout << "Test 4:\n\n";
    {
        std::cout << "\tInputs :\n";
        key = new const unsigned char[32] { 
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
            0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,      
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
            0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4   
        };
        std::cout << "\t\t Key : 603deb10 15ca71be 2b73aef0 857d7781 1f352c07 3b6108d7 2d9810a3 0914dff4\n\n";

        AES aes(key);
        delete[] key;

        data = new const unsigned char[16] {    
           0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
           0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef
        };

        std::cout << "\t\t Data : 30c81c46 a35ce411 e5fbc119 1a0a52ef\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\tb6ed21b9 9ca6f4f9 f153e7b1 beafed1d\n\n";
        
        output.resize(16);
        aes.EncryptECB(data, output.data(), 16);
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), 16);

        aes.DecryptECB(output.data(), output.data(), 16);

        std::cout << "\n\n\tDecrypted Output :\n\t";
        PrintByteArray(output.data(), 16);
    }

    std::cout << "\n\t------------------------------\n";
    
    std::cout << "Test 5:\n\n";
    {
        std::cout << "\tInputs :\n";
        key = new const unsigned char[32] { 
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
            0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,      
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
            0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4   
        };
        std::cout << "\t\t Key : 603deb10 15ca71be 2b73aef0 857d7781 1f352c07 3b6108d7 2d9810a3 0914dff4\n\n";

        AES aes(key);
        delete[] key;

        data = new const unsigned char[16] {    
           0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
           0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
        };

        std::cout << "\t\t Data : f69f2445 df4f9b17 ad2b417b e66c3710\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t23304b7a 39f9f3ff 067d8d8f 9e24ecc7\n\n";
        
        output.resize(16);
        aes.EncryptECB(data, output.data(), 16);
        delete[] data;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), 16);

        aes.DecryptECB(output.data(), output.data(), 16);

        std::cout << "\n\n\tDecrypted Output :\n\t";
        PrintByteArray(output.data(), 16);
    }
    
}

void CombinedUsageExample()
{
    using namespace KeyExchange;
    std::cout << "\n\n===== Diffie Hellman Key Exchange =====\n\n";
    BigUInt<PRIVATE_KEY_SIZE>   private1;
    BigUInt<PUBLIC_KEY_SIZE>    public1;

    BigUInt<PRIVATE_KEY_SIZE>   private2;
    BigUInt<PUBLIC_KEY_SIZE>    public2;

    DiffieHellman::GenerateKeyPair(private1, public1);
    std::cout << "Client1 Private Key: \n" << private1 << "\n\n";
    std::cout << "Client1 Public  Key: \n" << public1 << "\n\n";

    std::cout << '\n';

    DiffieHellman::GenerateKeyPair(private2, public2);
    std::cout << "Client2 Private Key: \n" << private2 << "\n\n";
    std::cout << "Client2 Public  Key: \n" << public2 << "\n\n";

    std::cout << "Send Public Keys over some network\n\n";

    auto shared1 = DiffieHellman::GenerateSharedKey(public2, private1);
    std::cout << "Client1 Shared Secret: \n" << shared1 << "\n\n";
    auto shared2 = DiffieHellman::GenerateSharedKey(public1, private2);
    std::cout << "Client2 Shared Secret: \n" << shared2 << "\n\n";

    std::cout << "Hash shared secret for an encryption key\n";
    auto hashedSecret1 = Hash::SHA256().Hash(reinterpret_cast<unsigned char*>(shared1.Get64BitArray()), PUBLIC_KEY_SIZE / 8);
    std::cout << "\nClient1 Hashed Secret:\n";
    PrintByteArray(hashedSecret1.data(), Hash::SHA256::OUTPUT_SIZE);

    auto hashedSecret2 = Hash::SHA256().Hash(reinterpret_cast<unsigned char*>(shared2.Get64BitArray()), PUBLIC_KEY_SIZE / 8);
    std::cout << "\nClient2 Hashed Secret:\n";
    PrintByteArray(hashedSecret2.data(), Hash::SHA256::OUTPUT_SIZE);
    
    std::cout << "\nEncrypt message with hashed secret\n";

    unsigned char client1data[128] = "Some data from client1 which will be sent over some network once encrypted and marked with and HMAC";
    std::cout << "Message: " << client1data << "\n";
    Encryption::AES client1AES(hashedSecret1.data());
    client1AES.EncryptECB(client1data, client1data, 99);

    const uint32_t messageLength = (99 / 16 + 1) * 16;
    std::cout << "\nEncrypted Message: \n";
    PrintByteArray(client1data, messageLength);

    std::cout << "\nCompute HMAC for the message\n";
    auto hmac = Hash::HMAC::HMAC_SHA256(hashedSecret1.data(), Hash::SHA256::OUTPUT_SIZE, client1data, messageLength);
    std::cout << "\nHMAC:\n";
    PrintByteArray(hmac.data(), hmac.size());

    std::cout << "\nAppend HMAC\n";

    std::vector<uint8_t> completeMessage;
    completeMessage.resize(messageLength + Hash::SHA256::OUTPUT_SIZE, 0);
    memcpy(&completeMessage[0], client1data, messageLength);
    memcpy(&completeMessage[0] + messageLength, hmac.data(), hmac.size());

    std::cout << "\nComplete message :\n";
    PrintByteArray(completeMessage.data(), completeMessage.size());
    
    std::cout << "\nSend complete message to client2\n";


    std::vector<uint8_t> client2Message;
    client2Message.resize(completeMessage.size() - Hash::SHA256::OUTPUT_SIZE, 0);
    std::array<uint8_t, Hash::SHA256::OUTPUT_SIZE> receivedHMAC{};
    memcpy(&client2Message[0], completeMessage.data(), client2Message.size());
    memcpy(&receivedHMAC[0], completeMessage.data() + client2Message.size(), Hash::SHA256::OUTPUT_SIZE);

    std::cout << "\nClient2 received message\n";
    PrintByteArray(&client2Message[0], client2Message.size());
    
    std::cout << "\nClient2 received HMAC\n";
    PrintByteArray(&receivedHMAC[0], receivedHMAC.size());

    std::cout << "\nClient2 checks HMAC\n";
    auto client2HMAC = Hash::HMAC::HMAC_SHA256(hashedSecret2.data(), Hash::SHA256::OUTPUT_SIZE, &client2Message[0], client2Message.size());
    std::cout << "\nClient 2 computed HMAC:\n";
    PrintByteArray(client2HMAC.data(), client2HMAC.size());

    if(memcmp(client2HMAC.data(), receivedHMAC.data(), Hash::SHA256::OUTPUT_SIZE) != 0)
    {
        std::cout << "\nInvalid HMAC\n";
        return;
    }
    std::cout << "\nHMAC is valid!\n\n";
    Encryption::AES client2AES(hashedSecret2.data());

    client2AES.DecryptECB(client2Message.data(), client2Message.data(), client2Message.size());

    std::cout << "Decrypted Message:\n";
    std::cout << client2Message.data();

}
