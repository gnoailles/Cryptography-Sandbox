#pragma once

template <unsigned BitCount>
bool BigUInt<BitCount>::IsZero() const
{
    for (int i = 0; i < ULL_COUNT; ++i)
    {
        if (number[i] != 0)
            return false;
    }
    return true;
}

template <unsigned BitCount>
bool BigUInt<BitCount>::IsOdd() const
{
    return (number[0] & 1);
}

template <unsigned BitCount>
bool BigUInt<BitCount>::operator==(uint64_t value) const
{
    for (int i = ULL_COUNT - 1; i > 0; --i)
    {
        if (number[i] != 0)
            return false;
    }
    return number[0] == value;
}

template <unsigned BitCount>
bool BigUInt<BitCount>::operator==(const BigUInt& other) const
{
    if (other.ULL_COUNT != ULL_COUNT)
        return false;
    for (int i = 0; i < ULL_COUNT; ++i)
    {
        if (number[i] != other.number[i])
            return false;
    }
    return true;
}

template <unsigned BitCount>
int BigUInt<BitCount>::Compare(const BigUInt<BitCount>& other) const
{
    for (int i = ULL_COUNT - 1; i >= 0; --i)
    {
        if (number[i] > other.number[i])
            return 1;
        if (number[i] < other.number[i])
            return -1;
    }
    return 0;
}