#pragma once

template <unsigned BitCount>
BigUInt<BitCount>::BigUInt(const uint64_t value)
{
    number[0] = value;
}

template <unsigned BitCount>
BigUInt<BitCount>::BigUInt(const uint64_t array[], unsigned size)
{
    if (ULL_COUNT >= size)
        memcpy_s(number, ULL_COUNT * 8, array, size * 8);
    else
        memcpy_s(number, ULL_COUNT * 8, array, ULL_COUNT * 8);
}

template <unsigned BitCount>
BigUInt<BitCount>::BigUInt(const uint32_t array[], unsigned size)
{
    for (unsigned int i = 0, j = 0; i < ULL_COUNT && j < size; ++i, ++j)
    {
        number[i] = array[j];
        number[i] <<= 32;
        ++j;
        if (j < size)
            number[i] |= array[j];
    }
}

template <unsigned BitCount>
BigUInt<BitCount>::BigUInt(std::initializer_list<uint64_t> list)
{
    auto it = list.begin();
    for (int i = 0; i < list.size(); ++i, ++it)
    {
        number[i] = *it;
    }
}

template <unsigned BitCount>
BigUInt<BitCount>::BigUInt(std::initializer_list<uint32_t> list)
{
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i)
    {
        number[i] = *it;
        number[i] <<= 32;
        ++it;
        if (it == list.end())
            break;
        number[i] |= *it;
    }
}

template <unsigned BitCount>
BigUInt<BitCount>::BigUInt(const BigUInt<BitCount>& other)
{
    memcpy_s(number, ULL_COUNT * 8, other.number, other.ULL_COUNT * 8);
}

template <unsigned BitCount>
template <unsigned OtherBitCount>
BigUInt<BitCount>::BigUInt(const BigUInt<OtherBitCount>& other)
{
    if constexpr (ULL_COUNT >= other.ULL_COUNT)
        memcpy_s(number, ULL_COUNT * 8, other.number, other.ULL_COUNT * 8);
    else
        memcpy_s(number, ULL_COUNT * 8, other.number, ULL_COUNT * 8);
}

template <unsigned BitCount>
BigUInt<BitCount>& BigUInt<BitCount>::operator=(const BigUInt<BitCount>& other)
{
    memcpy_s(number, ULL_COUNT * 8, other.number, other.ULL_COUNT * 8);
    return *this;
}

template <unsigned BitCount>
template <unsigned OtherBitCount>
BigUInt<BitCount>& BigUInt<BitCount>::operator=(const BigUInt<BitCount>& other)
{
    if constexpr (ULL_COUNT >= other.ULL_COUNT)
        memcpy_s(number, ULL_COUNT * 8, other.number, other.ULL_COUNT * 8);
    else
        memcpy_s(number, ULL_COUNT * 8, other.number, ULL_COUNT * 8);
    return *this;
}

template <unsigned BitCount>
BigUInt<BitCount> BigUInt<BitCount>::Random()
{
    static std::mt19937 randEngine;
    static std::uniform_int_distribution<uint64_t> uintDist;

    BigUInt<BitCount> res;
    for (int i = 0; i < ULL_COUNT; ++i)
    {
        res.number[i] = uintDist(randEngine);
    }
    return res;
}