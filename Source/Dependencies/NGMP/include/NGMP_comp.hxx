#pragma once

template <unsigned BitCount>
bool NGMP<BitCount>::IsZero() const
{
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        if (number[i] != 0)
            return false;
    }
    return true;
}

template <unsigned BitCount>
bool NGMP<BitCount>::IsOdd() const
{
    return (number[0] & 1);
}

template <unsigned BitCount>
bool NGMP<BitCount>::operator==(uint64_t value) const
{
    for (int i = MAX_LIMB_COUNT - 1; i > 0; --i)
    {
        if (number[i] != 0)
            return false;
    }
    return number[0] == value;
}

template <unsigned BitCount>
bool NGMP<BitCount>::operator==(const NGMP& other) const
{
    if (other.MAX_LIMB_COUNT != MAX_LIMB_COUNT)
        return false;
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        if (number[i] != other.number[i])
            return false;
    }
    return true;
}

template <unsigned BitCount>
int NGMP<BitCount>::Compare(const NGMP<BitCount>& other) const
{
    for (int i = MAX_LIMB_COUNT - 1; i >= 0; --i)
    {
        if (number[i] > other.number[i])
            return 1;
        if (number[i] < other.number[i])
            return -1;
    }
    return 0;
}