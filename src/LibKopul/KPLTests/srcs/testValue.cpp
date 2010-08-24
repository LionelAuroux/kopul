#include "test.h"
#include "Register.h"
#include "StaticArray.h"
#include "Bitfield.h"
#include "Value.h"

using namespace kpl;

namespace NamespaceTestValue
{

    bool test1()
    {
        //Creation d'un module
        Register    reg;
        Bitfield    bit6(6);
        Value       value(bit6, (char)4);

        // Construction du register
        bit6.SetName("i6");
        value.SetName("value4");
        reg << bit6 << value;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        char    *test = new char[5];
        char    *tmp = test;
        char    c;
        c = 2;

        listFunction[bit6].encode(&test, &c);
        listFunction[value].encode(&test);
        listFunction[bit6].encode(&test, &c);
        listFunction[bit6].encode(&test, &c);

        test = tmp;
        listFunction[bit6].decode(&test, &c);
        if (listFunction[value].decode(&test) != 0)
        {
            std::cout << "decode doesn't succeede" << std::endl;
            return (false);
        }
        listFunction[bit6].decode(&test, &c);
        if (listFunction[value].decode(&test) != -1)
        {
            std::cout << "decode doesn't failed" << std::endl;
            return (false);
        }
        return (true);
    }

    bool test2()
    {
        //Creation d'un module
        Register    reg;
        Bitfield    bit8(8);
        StaticArray array(bit8, 7);
        Value       value_coucou(array, std::string("coucou"));
        Value       value_d(bit8, 'd');

        // Construction du register
        value_coucou.SetName("value_coucou");
        value_d.SetName("value_d");
        reg << value_coucou << value_d;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        char    *test = new char[8];
        char    *tmp = test;

        listFunction[value_coucou].encode(&test);
        test = tmp;
        if (listFunction[value_coucou].decode(&test) != 0)
        {
            std::cout << "Test doesn't succeed" << std::endl;
            return (false);
        }
        test = tmp;
        if (listFunction[value_d].decode(&test) != -1)
        {
            std::cout << "Test doesn't failed" << std::endl;
            return (false);
        }

        return (true);
    }
}

using namespace NamespaceTestValue;

bool testValue()
{
    return (V(test1(), "test1") && V(test2(), "test2"));
}
