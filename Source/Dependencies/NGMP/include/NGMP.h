#pragma once
#include <cstdint>
#include <ostream>
#include <initializer_list>
#include <corecrt_memcpy_s.h>
#include <random>
#include <cassert>
#include <sstream>

template<unsigned int BitCount>
class NGMP
{
    static_assert(BitCount % 64 == 0, "Only multiples of 64 are supported");
    template<unsigned int OtherBitCount>
    friend class NGMP;

private:
    static const unsigned int MAX_LIMB_COUNT = BitCount / 64;
    uint64_t number[MAX_LIMB_COUNT] = {0};

public:

#pragma region Constructors & Assignments
    NGMP()   = default;
    ~NGMP()  = default;
    NGMP(const uint64_t value);
    NGMP(const uint64_t array[], unsigned int size);
    NGMP(const uint32_t array[], unsigned int size);

    NGMP(std::initializer_list<uint64_t> list);
    NGMP(std::initializer_list<uint32_t> list);
    NGMP(const NGMP<BitCount>& other);

    template<unsigned int OtherBitCount>
    NGMP(const NGMP<OtherBitCount>& other);

    NGMP& operator=(const NGMP<BitCount>& other);
    template<unsigned int OtherBitCount>
    NGMP& operator=(const NGMP<BitCount>& other);

    static NGMP<BitCount> Random();
#pragma endregion 

#pragma region Comparison operators
    bool IsZero() const;
    bool IsOdd() const;

    bool operator==(uint64_t value) const;
    bool operator==(const NGMP& other) const;

    int Compare(const NGMP<BitCount>& other) const;
#pragma endregion 

#pragma region Bitwise Operations
    NGMP<BitCount>& LeftShift(uint64_t value = 1);
    NGMP<BitCount>& RightShift(uint64_t value = 1);

    uint64_t     FindHighestBit() const;
    unsigned int FindUsedLimbCount() const;

    NGMP operator~() const;
    NGMP& operator~();
#pragma endregion 

#pragma region Arithmetic Operations
    NGMP operator+(const uint64_t other) const;
    NGMP operator+(const NGMP other) const;
    NGMP& operator+=(const uint64_t other);
    NGMP& operator+=(const NGMP other);


    NGMP operator-(const uint64_t other) const;
    //Using 2s complement for BigInt subtraction
    NGMP operator-(const NGMP& other) const;
    NGMP& operator-=(const NGMP& other);

    //Binary (peasant) multiplication 
    NGMP LongMultiplication(NGMP b) const;
    NGMP& LongMultiplication(NGMP b);
    NGMP Karatsuba(NGMP b) const;
    NGMP operator*(NGMP b) const;
    NGMP& operator*=(NGMP b);

    template<unsigned int OtherBitCount, unsigned int Remainder>
    NGMP<BitCount> Divide(const NGMP<OtherBitCount>& b, NGMP<Remainder>& r) const;

    template<unsigned int OtherBitCount>
    NGMP<BitCount> operator/(NGMP<OtherBitCount> b) const;
    template<unsigned int OtherBitCount>
    NGMP<BitCount>& operator/=(NGMP<OtherBitCount> b);

    template<unsigned int OtherBitCount>
    NGMP<BitCount> operator%(NGMP<OtherBitCount> b) const;
    template<unsigned int OtherBitCount>
    NGMP<BitCount>& operator%=(NGMP<OtherBitCount> b);

    NGMP Power(uint64_t power) const;
    NGMP Power(NGMP power) const;
#pragma  endregion 

#pragma region Modular Arithmetic
    #pragma region Multiplication

    /**
     * \brief Modular multiplication
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of modulus in bits
     * \param b value multiplied by instance
     * \param mod modulus applied to multiplication
     * \return ref to instance
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    NGMP& MulMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod);
    /**
     * \brief Modular multiplication
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of modulus in bits
     * \param b value multiplied by instance
     * \param mod modulus applied to multiplication
     * \return instance * b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    NGMP MulMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod) const;

    /**
     * \brief Modular multiplication
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of modulus in bits
     * \param a value multiplied by b
     * \param b value multiplied by a
     * \param mod modulus applied to multiplication
     * \return a * b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    static NGMP MulMod(NGMP<BitCount> a, NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod);
    #pragma  endregion 

    #pragma region Exponentiation
     /**
     * \brief Modular exponentiation
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of modulus in bits
     * \param b exponent
     * \param mod modulus applied to exponentiation
     * \return ref to instance
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    NGMP& PowMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod);
    /**
     * \brief Modular exponentiation
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of modulus in bits
     * \param b exponent
     * \param mod modulus applied to exponentiation
     * \return instance ^ b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    NGMP PowMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod) const;

    /**
     * \brief Modular exponentiation
     * \tparam OtherBitCount size of B in bits
     * \tparam ModBitCount size of modulus in bits
     * \param a base
     * \param b exponent
     * \param mod modulus applied to exponentiation
     * \return a ^ b % mod
     */
    template<unsigned int OtherBitCount, unsigned int ModBitCount>
    static NGMP PowMod(NGMP<BitCount> a, NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod);
    #pragma  endregion 
#pragma  endregion 

#pragma region Utils

    //TODO Add casts to uint types

    uint64_t NumberOfDigits() const
    {
        return 1 + FindHighestBit() * log10(2);
    }

    friend std::ostream& operator<<(std::ostream& os, const NGMP<BitCount>& p_bigUInt)
    {
        for (int i = 0; i < p_bigUInt.MAX_LIMB_COUNT; ++i)
        {
            os << std::hex << std::uppercase << p_bigUInt.number[i] << " ";
            if((i + 1) % 6 == 0)
                os << '\n';
        }
        return os;
    }

    std::string ToString() const
    {
        std::ostringstream ss;
        for (int i = 0; i < MAX_LIMB_COUNT; ++i)
        {
            ss << std::hex << std::uppercase << number[i] << " ";
            if((i + 1) % 6 == 0)
                ss << '\n';
        }
        return ss.str();
    }

    uint64_t* Get64BitArray()
    {
        return number;
    }
#pragma endregion
};



#include "NGMP_ctor_assign.hxx"
#include "NGMP_comp.hxx"
#include "NGMP_bitwise.hxx"
#include "NGMP_arithmetic.hxx"
#include "NGMP_mod_arithmetic.hxx"
