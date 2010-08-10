#include "Register.h"
#include "Bitfield.h"
#include "StaticArray.h"
#include "StaticStruct.h"

using namespace kpl;

int main()
{
	//Creation d'un module
    Register        reg;
    StaticStruct    sstruct;
    Bitfield        bit(1);
    Bitfield        bit4(4);
    StaticArray     staticArray(bit4, 4);

    sstruct.SetName("struct");
    bit.SetName("i1");
    staticArray.SetName("array");
    sstruct << bit4 << bit4 << staticArray << bit4 << bit4 << bit4;
    reg << staticArray << sstruct << bit;
    reg.Dump();
    return (0);
}
