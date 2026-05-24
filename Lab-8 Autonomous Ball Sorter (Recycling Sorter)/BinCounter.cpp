#include "BinCounter.h"

BinCounter::BinCounter()
{
    reset();
}

void BinCounter::increment(int bin)
{
    if (bin >= 1 && bin <= 4)
    {
        _counts[bin - 1]++;
    }
}

void BinCounter::reset()
{
    for (int i = 0; i < 4; i++)
    {
        _counts[i] = 0;
    }
}

int BinCounter::get(int bin) const
{
    if (bin >= 1 && bin <= 4)
    {
        return _counts[bin - 1];
    }

    return 0;
}
