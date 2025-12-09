#include "shaders.hh"

void checker(int x, int y, int frame, unsigned char& r, unsigned char& g, unsigned char& b)
{
    bool is_checker = ((x + frame) / 144 + (y + frame) / 144) % 2 != 0;

    if (is_checker)
    {
        r = 0xFF;
        g = 0x00;
        b = 0x00;
    }
    else
    {
        r = 0x00;
        g = 0x00;
        b = 0x00;
    }
}
