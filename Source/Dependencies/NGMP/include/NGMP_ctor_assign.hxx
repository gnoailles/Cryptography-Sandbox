#pragma once

template <unsigned BitCount>
NGMP<BitCount>::NGMP(const uint64_t value)
{
    number[0] = value;
}

template <unsigned BitCount>
NGMP<BitCount>::NGMP(const uint64_t array[], unsigned size)
{
    if (MAX_LIMB_COUNT >= size)
        memcpy_s(number, MAX_LIMB_COUNT * 8, array, size * 8);
    else
        memcpy_s(number, MAX_LIMB_COUNT * 8, array, MAX_LIMB_COUNT * 8);
}

template <unsigned BitCount>
NGMP<BitCount>::NGMP(const uint32_t array[], unsigned size)
{
    for (unsigned int i = 0, j = 0; i < MAX_LIMB_COUNT && j < size; ++i, ++j)
    {
        number[i] = array[j];
        number[i] <<= 32;
        ++j;
        if (j < size)
            number[i] |= array[j];
    }
}

template <unsigned BitCount>
NGMP<BitCount>::NGMP(std::initializer_list<uint64_t> list)
{
    auto it = list.begin();
    for (int i = 0; i < list.size(); ++i, ++it)
    {
        number[i] = *it;
    }
}

template <unsigned BitCount>
NGMP<BitCount>::NGMP(std::initializer_list<uint32_t> list)
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
NGMP<BitCount>::NGMP(const NGMP<BitCount>& other)
{
    memcpy_s(number, MAX_LIMB_COUNT * 8, other.number, other.MAX_LIMB_COUNT * 8);
}

template <unsigned BitCount>
template <unsigned OtherBitCount>
NGMP<BitCount>::NGMP(const NGMP<OtherBitCount>& other)
{
    if constexpr (MAX_LIMB_COUNT >= other.MAX_LIMB_COUNT)
        memcpy_s(number, MAX_LIMB_COUNT * 8, other.number, other.MAX_LIMB_COUNT * 8);
    else
        memcpy_s(number, MAX_LIMB_COUNT * 8, other.number, MAX_LIMB_COUNT * 8);
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::operator=(const NGMP<BitCount>& other)
{
    memcpy_s(number, MAX_LIMB_COUNT * 8, other.number, other.MAX_LIMB_COUNT * 8);
    return *this;
}

template <unsigned BitCount>
template <unsigned OtherBitCount>
NGMP<BitCount>& NGMP<BitCount>::operator=(const NGMP<BitCount>& other)
{
    if constexpr (MAX_LIMB_COUNT >= other.MAX_LIMB_COUNT)
        memcpy_s(number, MAX_LIMB_COUNT * 8, other.number, other.MAX_LIMB_COUNT * 8);
    else
        memcpy_s(number, MAX_LIMB_COUNT * 8, other.number, MAX_LIMB_COUNT * 8);
    return *this;
}

template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::Random()
{
    static std::mt19937 randEngine;
    static std::uniform_int_distribution<uint64_t> uintDist;

    NGMP<BitCount> res;
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        res.number[i] = uintDist(randEngine);
    }
    return res;
}