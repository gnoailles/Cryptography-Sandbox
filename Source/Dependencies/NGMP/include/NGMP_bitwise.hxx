#pragma once
#include <bitset>

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::LeftShift(uint64_t value)
{
    if(value == 0)
        return * this;
    
    if(value > 64)
    {
        LeftShift(static_cast<uint64_t>(std::ceil(value / 2)));
        LeftShift(value - static_cast<uint64_t>(std::ceil(value / 2)));
        return *this;
    }

    uint64_t mask = (((uint64_t)1 << value) - 1);
    for (int i = MAX_LIMB_COUNT - 1; i > 0; --i)
    {
        //Move previous block MSD to current block LSD
        number[i] = (number[i] << value) | ((number[i - 1] >> (64 - value)) & mask);
    }
    number[0] <<= value;
    return *this;
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::RightShift(uint64_t value)
{
    if(value == 0)
        return *this;

    if(value > 64)
    {
        RightShift(static_cast<uint64_t>(std::ceil(value / 2)));
        RightShift(value - static_cast<uint64_t>(std::ceil(value / 2)));
        return *this;
    }

    for (int i = 0; i < MAX_LIMB_COUNT - 1; ++i)
    {
        //Move next block LSD to current block MSD
        number[i] = (number[i] >> value) | ((number[i + 1] << (64 - value)));
    }
    number[MAX_LIMB_COUNT - 1] >>= value;
    return *this;
}

template <unsigned BitCount>
uint64_t  NGMP<BitCount>::FindHighestBit() const
{
    for (int i = MAX_LIMB_COUNT - 1; i >= 0; --i)
    {
        if(number[i] == 0)
            continue;
        for(int j = 63; j >=0; --j)
        {
            if(number[i] & (uint64_t(1) << j))
                return i * 64 + j + 1;
        }
    }
    return 0;
}

template <unsigned BitCount>
unsigned int  NGMP<BitCount>::FindUsedLimbCount() const
{
    for (int i = MAX_LIMB_COUNT - 1; i >= 0; --i)
    {
        if(number[i] != 0)
            return i + 1;
    }
    return 0;
}


template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::operator~() const
{
    NGMP<BitCount> res;
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        res.number[i] = ~number[i];
    }
    return res;
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::operator~()
{
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        number[i] = ~number[i];
    }
    return *this;
}