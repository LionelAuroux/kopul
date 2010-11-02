#include "test.h"
#include "Register.h"
#include "Bitfield.h"
#include "Switch.h"
#include "Variable.h"
#include "ConstantValue.h"

using namespace kpl;

namespace NamespaceTestSwitch
{

    bool test1()
    {
        Bitfield        bit2(2);
        Bitfield        bit4(4);
        Bitfield        bit5(5);
        Bitfield        bit8(8);
        Bitfield        bit16(16);
        Bitfield        bit32(32);
        Variable        var("var", bit32);
        ConstantValue   val0(bit32, 0);
        ConstantValue   val1(bit32, 1);
        ConstantValue   val2(bit32, 2);
        ConstantValue   val3(bit32, 3);
        ConstantValue   val4(bit32, 4);
        ConstantValue   val5(bit32, 5);
        Switch          sw;
        Switch          sw2;

        sw  << ((var <= val0), bit2)
            << ((var >= val0 && var <= val1), bit4)
            << ((var != val0 && var != val1 && var <= val3), bit5)
            << (((var > val2 && var < val4) || var == val5), bit16);
        sw2 << (bit2, var <= val0)
            << (bit4, var >= val0 && var <= val1)
            << (bit5, var != val0 && var != val1 && var <= val3)
            << (bit16, (var > val2 && var < val4) || var == val5);
        std::cout << sw2.ToString() << std::endl;
        //std::cout << sw.ToString() << std::endl;
        return (sw2.Equals(sw));
    }

    bool test2()
    {
        Register        reg;
        Bitfield        bit2(2);
        Bitfield        bit4(4);
        Bitfield        bit5(5);
        Bitfield        bit8(8);
        Bitfield        bit16(16);
        Bitfield        bit32(32);
        Variable        var1("var1", bit32);
        Variable        var2("var2", bit32);
        Variable        var3("var3", bit32);
        ConstantValue   val0(bit32, 0);
        ConstantValue   val1(bit32, 1);
        ConstantValue   val2(bit32, 2);
        ConstantValue   val3(bit32, 3);
        ConstantValue   val4(bit32, 4);
        ConstantValue   val5(bit32, 5);
        Switch          sw;

        sw  << ((var1 <= val0), bit2)
            << ((var1 >= val0 && var1 <= val1), bit4)
            << ((var1 != val0 && var2 != val1 && var3 <= val3), bit5)
            << (((var3 > val2 && var3 < val4) || var3 == val5), bit16);
        std::cout << sw.ToString() << std::endl;
        reg << sw;
        reg.Dump(MEMORY_MODE);
        return (true);
    }
}

using namespace NamespaceTestSwitch;

bool testSwitch()
{
    return (V(test1(), "test1") && V(test2(), "test2"));
}
