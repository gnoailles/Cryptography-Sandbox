#pragma once

template <unsigned BitCount>
void BigUInt<BitCount>::LeftShift()
{
    for (int i = ULL_COUNT - 1; i > 0; --i)
    {
        //Move previous block MSD to current block LSD
        number[i] = (number[i] << 1) | ((number[i - 1] >> 63) & 1);
    }
    number[0] <<= 1;
}

template <unsigned BitCount>
void BigUInt<BitCount>::RightShift()
{
    for (int i = 0; i < ULL_COUNT - 1; ++i)
    {
        //Move next block LSD to current block MSD
        number[i] = (number[i] >> 1) | ((number[i + 1] << 63));
    }
    number[ULL_COUNT - 1] >>= 1;
}


template <unsigned BitCount>
BigUInt<BitCount> BigUInt<BitCount>::operator~() const
{
    BigUInt<BitCount> res;
    for (int i = 0; i < ULL_COUNT; ++i)
    {
        res.number[i] = ~number[i];
    }
    return res;
}

template <unsigned BitCount>
BigUInt<BitCount>& BigUInt<BitCount>::operator~()
{
    for (int i = 0; i < ULL_COUNT; ++i)
    {
        number[i] = ~number[i];
    }
    return *this;
}