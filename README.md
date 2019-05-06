# Cryptography Sandbox

This sandbox is for a learning purpose only and implemented algorithms are not designed to be completely secure or optimized.

The currently implemented algorithms are :
*   Key Exchange
    * Diffie-Hellman
* Hashing
    *   SHA-256
    *   HMAC
* Encryption
    *   AES-256


## Diffie-Hellman
Diffie-Hellman-Merkle key exchange implementation using a custom [Multi Precision Integer](https://github.com/gnoailles/MultiPrecision) library.

Prime of size 1024, 1536 and 2048 are presets based on [RFC2409](https://tools.ietf.org/html/rfc2409#section-6.2) and [RFC3526](https://tools.ietf.org/html/rfc3526#section-2)

### Example :

`PUBLIC_KEY_SIZE` is set to 2048

`PRIVATE_KEY_SIZE` is set to 256

```c++
    NGMP<PRIVATE_KEY_SIZE>   private1;
    NGMP<PUBLIC_KEY_SIZE>    public1;

    NGMP<PRIVATE_KEY_SIZE>   private2;
    NGMP<PUBLIC_KEY_SIZE>    public2;

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
```

### Output :
![alt text](https://i.imgur.com/9tugRCo.png "Diffie-Hellman Output Screenshot")

## SHA-256
The SHA-256 implementation currently supports only Little Endian architecture. A rework is planned for a more generic approach.

### Example :

```c++
    std::cout << "\tInput :\n";
    const unsigned char* input = new unsigned char[1000000];
    memset((void*)input, 'a', 1000000);

    std::cout << "\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.... (1,000,000 bytes)\n\n";
    std::cout << "\tExpected Output :\n";
    std::cout << "\tcdc76e5c 9914fb92 81a1c7e2 84d73e67 f1809a48 a497200e 046d39cc c7112cd0\n\n";

    std::array<uint8_t, SHA256::OUTPUT_SIZE> output = SHA256().Hash(input, 1000000);
    delete[] input;
    std::cout << "\tOutput :\n\t";
    PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
```

### Output :
![alt text](https://i.imgur.com/jxvFcfC.png "SHA-256 Output Screenshot")

## HMAC_SHA-256
HMAC using SHA-256 as hashing function.

### Example :

```c++
        std::cout << "\tInputs :\n";

        const unsigned char* key = new const unsigned char[131]{};
        memset((void*)key, 0xaa, 131);
        const unsigned char* data = reinterpret_cast<const unsigned char*>("This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.");
        std::cout << "\t\t Key : 0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa....aa (131 bytes)\n\n";
        std::cout << "\t\t Data : \"" << data << "\"\n\n";

        std::cout << "\tExpected Output :\n";
        std::cout << "\t9b09ffa7 1b942fcb 27635fbc d5b0e944 bfdc6364 4f071393 8a7f5153 5c3a35e2\n\n";

        std::array<uint8_t, SHA256::OUTPUT_SIZE> output = HMAC::HMAC_SHA256(key, 131, data, 152);
        delete[] key;
        std::cout << "\tOutput :\n\t";
        PrintByteArray(output.data(), SHA256::OUTPUT_SIZE);
```

### Output :
![alt text](https://i.imgur.com/Txld5J7.png "HMAC_SHA-256 Output Screenshot")

## AES-256
AES implementation is based on [Intel AES instructions set](https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf).

### Example :

```c++
        std::cout << "\tInputs :\n";
        const unsigned char* key = new const unsigned char[32] { 
            0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
            0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,      
            0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
            0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4   
        };
        std::cout << "\t\t Key : 603deb10 15ca71be 2b73aef0 857d7781 1f352c07 3b6108d7 2d9810a3 0914dff4\n\n";

        AES aes(key);
        delete[] key;

        const unsigned char* data = new const unsigned char[16] {    
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
```

### Output :
![alt text](https://i.imgur.com/TcG6q5X.png "AES-256 Output Screenshot")
