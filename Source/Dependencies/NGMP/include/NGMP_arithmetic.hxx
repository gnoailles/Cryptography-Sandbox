#pragma once

#pragma region Addition
template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::operator+(const uint64_t other) const
{
    NGMP<BitCount> res(*this);
    res.number[0] = number[0] + other;
    //Propagate overflow
    for (int i = 0; res.number[i] < number[i] || res.number[i] < other && i < MAX_LIMB_COUNT - 1; ++i)
    {
        res.number[i + 1] = number[i + 1] + 1;
    }
    return res;
}

template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::operator+(const NGMP other) const
{
    NGMP<BitCount> res;
    uint8_t overflow = 0;
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        res.number[i] = number[i] + other.number[i] + overflow;
        overflow = 0;
        if (res.number[i] < number[i] || res.number[i] < other.number[i])
            overflow = 1;
    }
    return res;
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::operator+=(const uint64_t other)
{
    uint64_t add = number[0] + other;
    //Propagate overflow
    int i;
    for (i = 0; add < number[i] || add < other && i < MAX_LIMB_COUNT - 1; ++i)
    {
        number[i] = add;
        add = number[i + 1] + 1;
    }
    number[i] = add;
    return *this;
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::operator+=(const NGMP other)
{
    uint8_t overflow = 0;
    for (int i = 0; i < MAX_LIMB_COUNT; ++i)
    {
        uint64_t add = number[i] + other.number[i] + overflow;
        overflow = 0;
        if (add < number[i] || add < other.number[i])
            overflow = 1;

        number[i] = add;
    }
    return *this;
}

#pragma endregion 

#pragma region Subtraction
template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::operator-(const uint64_t other) const
{
    if (other == 0)
        return *this;
    NGMP<BitCount> result = *this;
    result.number[0] -= other;
    return result;
}

template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::operator-(const NGMP& other) const
{
    if (other.IsZero())
        return *this;
    NGMP<BitCount> invertOther = ~other;
    invertOther += 1;
    return *this + invertOther;
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::operator-=(const NGMP& other)
{
    if (other.IsZero())
        return *this;
    NGMP<BitCount> invertOther = ~other;
    invertOther += 1;
    *this += invertOther;
    return *this;
}
#pragma endregion 

#pragma region Multiplication
template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::LongMultiplication(NGMP b) const
{
    if (b.IsZero())
        return b;

    NGMP<BitCount> a = *this;
    NGMP<BitCount> result;

    while (!a.IsZero())
    {
        if (a.IsOdd())
            result += b;

        a.RightShift();
        b.LeftShift();
    }

    return result;
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::LongMultiplication(NGMP b)
{
    if (b.IsZero())
    {
        memset(number, 0, BitCount / 8);
        return *this;
    }

    NGMP<BitCount> a = *this;
    NGMP<BitCount> result;

    while (!a.IsZero())
    {
        if (a.IsOdd())
            result += b;

        a.RightShift();
        b.LeftShift();
    }

    *this = result;
    return *this;
}


template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::Karatsuba(NGMP b) const
{
    NGMP a (*this);
    NGMP bTest (b);

    //If a or b is <= 32bit
    a.number[0] ^= a.number[0] & 0x7FFFFFFF;
    bTest.number[0] ^= bTest.number[0] & 0x7FFFFFFF;
    if(a.IsZero() || bTest.IsZero())
        return this->LongMultiplication(b);

    a = *this;

    const unsigned aHighestBit = a.FindHighestBit();
    const unsigned bHighestBit = b.FindHighestBit();
    uint64_t middle  = std::ceil(std::max(aHighestBit, bHighestBit) * 0.5f);
    uint64_t byteCount  = std::ceil(middle / 8.0f);
    NGMP<BitCount> lowA, lowB, highA, highB;

    memcpy(lowA.number,a.number, byteCount);
    memcpy(lowB.number,b.number, byteCount);
    for (uint64_t i = 0; i < ((byteCount * 8) - middle); ++i)
    {
        *lowA.number &= ~(1UL << ((byteCount* 8) - i));
        *lowB.number &= ~(1UL << ((byteCount* 8) - i));
    }
    memcpy(highA.number,reinterpret_cast<char*>(a.number) + (byteCount - 1), byteCount);
    memcpy(highB.number,reinterpret_cast<char*>(b.number) + (byteCount - 1), byteCount);

    *highA.number >>= middle - ((byteCount - 1) * 8);
    *highB.number >>= middle - ((byteCount - 1) * 8);

    NGMP z0 = lowA.Karatsuba(lowB);
    NGMP z1 = (lowA + highA).Karatsuba(lowB + highB);
    NGMP z2 = highA.Karatsuba(highB);


    //DEBUG Probably need to handle negative values
    return (z2.LeftShift(middle * 2))  + ((z1-z2-z0).LeftShift(middle)) +  z0;
}

template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::operator*(NGMP b) const
{
    // if (FindHighestBit() > 512 || b.FindHighestBit() > 512)
    //     return Karatsuba(b);
    // else
        return LongMultiplication(b);
}

template <unsigned BitCount>
NGMP<BitCount>& NGMP<BitCount>::operator*=(NGMP b)
{
    // if (FindHighestBit() > 512 || b.FindHighestBit() > 512)
    //     *this = Karatsuba(b);
    // else
        *this = LongMultiplication(b);

    return *this;
}
#pragma endregion 

#pragma region Division
template <unsigned int BitCount>
template <unsigned int OtherBitCount, unsigned int Remainder>
NGMP<BitCount> NGMP<BitCount>::Divide(const NGMP<OtherBitCount>& b, NGMP<Remainder>& r) const
{
    if(b.IsZero())
        throw std::overflow_error("Divide by zero exception");
    NGMP<BitCount> divisor = b;
    NGMP quotient;
    r = *this;

    if (divisor.Compare(r) > 0)
        return 0;
    while (divisor.Compare(r) <= 0 && !(divisor.number[divisor.MAX_LIMB_COUNT - 1] & ((uint64_t)1 << 63)) )
    {
        divisor.LeftShift();
    }

    if(!(divisor.number[divisor.MAX_LIMB_COUNT - 1] & ((uint64_t)1 << 63)))
        divisor.RightShift();
    
    while (r.Compare(b) >= 0)
    {
        if(r.Compare(divisor) >= 0)
        {
            r -= divisor;
            quotient.number[0] |= 1;
        }
        quotient.LeftShift();
        divisor.RightShift();
    }
    return quotient;
}

template <unsigned int BitCount>
template<unsigned int OtherBitCount>
NGMP<BitCount> NGMP<BitCount>::operator/(NGMP<OtherBitCount> b) const
{
    NGMP r;
    return Divide(b,r);
}
template <unsigned int BitCount>
template<unsigned int OtherBitCount>
NGMP<BitCount>& NGMP<BitCount>::operator/=(NGMP<OtherBitCount> b)
{
    NGMP r;
    *this = Divide(b,r);
    return *this;
}

template <unsigned int BitCount>
template<unsigned int OtherBitCount>
NGMP<BitCount> NGMP<BitCount>::operator%(NGMP<OtherBitCount> b) const
{
    NGMP r;
    Divide(b,r);
    return r;
}

template <unsigned int BitCount>
template<unsigned int OtherBitCount>
NGMP<BitCount>& NGMP<BitCount>::operator%=(NGMP<OtherBitCount> b)
{
    NGMP r;
    Divide(b,r);
    *this = r;
    return *this;
}
#pragma endregion 

#pragma region Exponent
template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::Power(uint64_t power) const
{
    NGMP result;
    result += 1;
    NGMP a = *this;


    while (power != 0)
    {
        const bool isOdd = power % 2;
        power >>= 1;

        if (isOdd)
            result *= a;

        a *= a;
    }

    return result;
}

template <unsigned BitCount>
NGMP<BitCount> NGMP<BitCount>::Power(NGMP power) const
{
    NGMP result;
    NGMP a = *this;
    result += 1;


    while (!power.IsZero())
    {
        const bool isOdd = power.IsOdd();
        power.RightShift();

        if (isOdd)
            result *= a;

        a *= a;
    }

    return result;
}
#pragma endregion 