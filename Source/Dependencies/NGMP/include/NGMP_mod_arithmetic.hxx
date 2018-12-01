#pragma once

#pragma region Multiplication

template <unsigned int BitCount>
template <unsigned int OtherBitCount, unsigned int ModBitCount>
 NGMP<BitCount>& NGMP<BitCount>::MulMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod)
{
    NGMP<BitCount * 2> maxValue = 2;
    maxValue = maxValue.Power(ModBitCount);
    maxValue -= 1;
    maxValue *= 2;

    NGMP<BitCount * 2> maxHandledSize = 2;
    maxHandledSize = maxHandledSize.Power(BitCount);
    maxHandledSize -= 1;

    // Instance should handle (mod-1)*2
    assert(maxValue.Compare(maxHandledSize) <= 0);
    
    NGMP<BitCount> result;

	while (!b.IsZero()) 
    {
		if (b.IsOdd()) 
        {
			result += *this;
            // Really naive and bad modulus
			while (result.Compare(mod) > 0)
			   result -= mod;
        }

        b.RightShift();
        LeftShift();
		while (Compare(mod) > 0)
		{
		   *this -= mod;
		}
    }
    *this = result;
    return *this;
}

template <unsigned int BitCount>
template <unsigned int OtherBitCount, unsigned int ModBitCount>
 NGMP<BitCount> NGMP<BitCount>::MulMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod) const
{
    NGMP<BitCount * 2> maxValue = 2;
    maxValue = maxValue.Power(ModBitCount);
    maxValue -= 1;
    maxValue *= 2;

    NGMP<BitCount * 2> maxHandledSize = 2;
    maxHandledSize = maxHandledSize.Power(BitCount);
    maxHandledSize -= 1;

    // Instance should handle (mod-1)*2
    assert(maxValue.Compare(maxHandledSize) <= 0);
    
    NGMP<OtherBitCount> result;
    NGMP a = *this;

	while (!b.IsZero()) 
    {
		if (b.IsOdd()) 
        {
			result += a;
            // Really naive and bad modulus
			while (result.Compare(mod) > 0)
			   result -= mod;
        }

        b.RightShift();
        a.LeftShift();
		while (a.Compare(mod) > 0)
		   a -= mod;
    }
    return result;
}

template <unsigned int BitCount>
template<unsigned int OtherBitCount, unsigned int ModBitCount>
NGMP<BitCount> NGMP<BitCount>::MulMod(NGMP<BitCount> a, NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod)
{
   NGMP<BitCount * 2> maxValue = 2;
    maxValue = maxValue.Power(ModBitCount);
    maxValue -= 1;
    maxValue *= 2;

    NGMP<BitCount * 2> maxHandledSize = 2;
    maxHandledSize = maxHandledSize.Power(BitCount);
    maxHandledSize -= 1;

    // Instance should handle (mod-1)*2
    assert(maxValue.Compare(maxHandledSize) <= 0);
    
    NGMP<OtherBitCount> result;

	while (!b.IsZero()) 
    {
		if (b.IsOdd()) 
        {
			result += a;
            if (result.Compare(mod) > 0)
			   result %= mod;
        }

        b.RightShift();
        a.LeftShift();
        if (a.Compare(mod) > 0)
		   a %= mod;
    }
    return result;
}

#pragma endregion

#pragma region Exponentiation
template <unsigned int BitCount>
template <unsigned int OtherBitCount, unsigned int ModBitCount>
 NGMP<BitCount>& NGMP<BitCount>::PowMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod)
{
    NGMP<BitCount * 2> maxValue = 2;
    maxValue = maxValue.Power(ModBitCount);
    maxValue -= 1;
    maxValue = maxValue.Power(2);

    NGMP<BitCount * 2> maxHandledSize = 2;
    maxHandledSize = maxHandledSize.Power(BitCount);
    maxHandledSize -= 1;

    // Instance should handle (mod-1)^2
    assert(maxValue.Compare(maxHandledSize) <= 0);
    
    NGMP<BitCount> result(1);

	while (!b.IsZero()) 
    {
		if (b.IsOdd()) 
			result = NGMP<BitCount>::MulMod(*this, result, mod);

        this->MulMod(*this, mod);
        b.RightShift();
    }
    *this = result;
    return *this;
}

template <unsigned int BitCount>
template <unsigned int OtherBitCount, unsigned int ModBitCount>
 NGMP<BitCount> NGMP<BitCount>::PowMod(NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod) const
{
    NGMP<BitCount * 2> maxValue = 2;
    maxValue = maxValue.Power(ModBitCount);
    maxValue -= 1;
    maxValue = maxValue.Power(2);

    NGMP<BitCount * 2> maxHandledSize = 2;
    maxHandledSize = maxHandledSize.Power(BitCount);
    maxHandledSize -= 1;

    // Instance should handle (mod-1)^2
    assert(maxValue.Compare(maxHandledSize) <= 0);
    
    NGMP<BitCount> result(1);
    NGMP<BitCount> a(*this);

	while (!b.IsZero()) 
    {
		if (b.IsOdd()) 
			result = NGMP<BitCount>::MulMod(a, result, mod);

        a.MulMod(a, mod);
        b.RightShift();
    }
    return result;
}

template <unsigned int BitCount>
template<unsigned int OtherBitCount, unsigned int ModBitCount>
NGMP<BitCount> NGMP<BitCount>::PowMod(NGMP<BitCount> a, NGMP<OtherBitCount> b, const NGMP<ModBitCount>& mod)
{
    NGMP<BitCount * 2> maxValue = 2;
    maxValue = maxValue.Power(ModBitCount);
    maxValue -= 1;
    maxValue = maxValue.Power(2);

    NGMP<BitCount * 2> maxHandledSize = 2;
    maxHandledSize = maxHandledSize.Power(BitCount);
    maxHandledSize -= 1;

    // Instance should handle (mod-1)^2
    assert(maxValue.Compare(maxHandledSize) <= 0);
    
    NGMP<BitCount> result(1);

	while (!b.IsZero()) 
    {
		if (b.IsOdd()) 
			result = NGMP<BitCount>::MulMod(a, result, mod);

        a.MulMod(a, mod);
        b.RightShift();
    }
    return result;
}
#pragma endregion 