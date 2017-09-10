
#include "types.h"

_u8 _byte(_u64 x,_u64 y)
{
    switch (y)
    {
        case 0: return (_u8)((x & 0x00000000000000FF)>>0 );
        case 1: return (_u8)((x & 0x000000000000FF00)>>8 );
        case 2: return (_u8)((x & 0x0000000000FF0000)>>16);
        case 3: return (_u8)((x & 0x00000000FF000000)>>24);
        case 4: return (_u8)((x & 0x000000FF00000000)>>32);
        case 5: return (_u8)((x & 0x0000FF0000000000)>>40);
        case 6: return (_u8)((x & 0x00FF000000000000)>>48);
        case 7: return (_u8)((x & 0xFF00000000000000)>>56);
        default: return (_u8)0;
    }
}