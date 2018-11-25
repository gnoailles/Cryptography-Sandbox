#pragma once
#include <cstdint>
#include <ostream>
#include <initializer_list>
#include <corecrt_memcpy_s.h>
#include <random>
#include <cassert>

template<unsigned int BitCount>
class BigUInt
{
    static_assert(BitCount % 64 == 0, "Only multiples of 64 are supported");
    template<unsigned int OtherBitCount>
    friend class BigUInt;

private:
    static const unsigned int ULL_COUNT = BitCount / 64;
    uint64_t number[ULL_COUNT] = {0};

public:

#pragma region Constructors & Assignments
    BigUInt()   = default;
    ~BigUInt()  = default;
    BigUInt(const uint64_t value);
    BigUInt(const uint64_t array[], unsigned int size);
    BigUInt(const uint32_t array[], unsigned int size);

    BigUInt(std::initializer_list<uint64_t> list);
    BigUInt(std::initializer_list<uint32_t> list);
    BigUInt(const BigUInt<BitCount>& other);

    template<unsigned int OtherBitCount>
    BigUInt(const BigUInt<OtherBitCount>& other);

    BigUInt& operator=(const BigUInt<BitCount>& other);
    template<unsigned int OtherBitCount>
    BigUInt& operator=(const BigUInt<BitCount>& other);

    static BigUInt<BitCount> Random();
#pragma endregion 

#pragma region Comparison operators
    bool IsZero() const;
    bool IsOdd() const;

    bool operator==(uint64_t value) const;
    bool operator==(const BigUInt& other) const;

    int Compare(const BigUInt<BitCount>& other) const;
#pragma endregion 

#pragma region Bitwise Operations
    void LeftShift();
    void RightShift();

    BigUInt operator~() const;
    BigUInt& operator~();
#pragma endregion 

#pragma region Arithmetic Operations
    BigUInt operator+(const uint64_t other) const;
    BigUInt operator+(const BigUInt other) const;
    BigUInt& operator+=(const uint64_t other);
    BigUInt& operator+=(const BigUInt other);


    BigUInt operator-(const uint64_t other) const;
    //Using 2s complement for BigInt subtraction
    BigUInt operator-(const BigUInt other) const;
    BigUInt& operator-=(const BigUInt other);

    //Binary (peasant) multiplication 
    BigUInt operator*(BigUInt b) const;
    BigUInt& operator*=(BigUInt b);

    BigUInt Power(uint64_t power) const;
    BigUInt Power(BigUInt power) const;

    //TODO Implement division and modulus
#pragma  endregion 

#pragma region Modular Arithmetic
    #pragma region Multiplication

    /**
     * \brief Modular multiplication
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of Modulo in bits
     * \param b value multiplied by instance
     * \param mod modulo applied to multiplication
     * \return ref to instance
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    BigUInt& MulMod(BigUInt<OtherBitCount> b, const BigUInt<ModBitCount>& mod);
    /**
     * \brief Modular multiplication
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of Modulo in bits
     * \param b value multiplied by instance
     * \param mod modulo applied to multiplication
     * \return instance * b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    BigUInt MulMod(BigUInt<OtherBitCount> b, const BigUInt<ModBitCount>& mod) const;

    /**
     * \brief Modular multiplication
     * \tparam OtherBitCount size of B in bits, used for internal multiplication, should handle (mod-1)*2
     * \tparam ModBitCount size of Modulo in bits
     * \param a value multiplied by b
     * \param b value multiplied by a
     * \param mod modulo applied to multiplication
     * \return a * b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    static BigUInt MulMod(BigUInt<BitCount> a, BigUInt<OtherBitCount> b, const BigUInt<ModBitCount>& mod);
    #pragma  endregion 

    #pragma region Exponentiation
     /**
     * \brief Modular exponentiation
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of Modulo in bits
     * \param b exponent
     * \param mod modulo applied to exponentiation
     * \return ref to instance
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    BigUInt& PowMod(BigUInt<OtherBitCount> b, const BigUInt<ModBitCount>& mod);
    /**
     * \brief Modular exponentiation
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of Modulo in bits
     * \param b exponent
     * \param mod modulo applied to exponentiation
     * \return instance ^ b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    BigUInt PowMod(BigUInt<OtherBitCount> b, const BigUInt<ModBitCount>& mod) const;

    /**
     * \brief Modular exponentiation
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of Modulo in bits
     * \param a base
     * \param b exponent
     * \param mod modulo applied to exponentiation
     * \return a ^ b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    static BigUInt PowMod(BigUInt<BitCount> a, BigUInt<OtherBitCount> b, const BigUInt<ModBitCount>& mod);
    #pragma  endregion 
#pragma  endregion 

#pragma region Convertions

    //TODO Add casts to uint types
    friend std::ostream& operator<<(std::ostream& os, const BigUInt<BitCount>& p_bigUInt)
    {
        for (int i = 0; i < p_bigUInt.ULL_COUNT; ++i)
        {
            os << std::hex << std::uppercase << p_bigUInt.number[i] << " ";
            if((i + 1) % 6 == 0)
                os << '\n';
        }
        return os;
    }
#pragma endregion
};



#include "BigUInt_ctor_assign.hxx"
#include "BigUInt_comp.hxx"
#include "BigUInt_bitwise.hxx"
#include "BigUInt_arithmetic.hxx"
#include "BigUint_mod_arithmetic.hxx"
