#pragma once

class BinCounter
{
private:
    int _counts[4];

public:
    BinCounter();

    void increment(int bin);
    void reset();

    int get(int bin) const;
};
