#include <iostream>
#include "DiffieHellman.h"
#include "BigUInt/BigUInt.h"

int main()
{

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

    std::cin.get();
}
