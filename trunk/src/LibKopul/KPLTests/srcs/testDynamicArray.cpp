#include "test.h"
#include "Variable.h"
#include "VariableIterator.h"
#include "Bitfield.h"
#include "Value.h"
#include "Register.h"
#include "DynamicArray.h"

using namespace kpl;

namespace NamespaceTestDynamicArray
{
    bool test1()
    {
        Bitfield        bit8(8);
        ConstantValue   end(bit8, '\0');
        Variable        lastBytes("LastBytes", bit8, true);
        DynamicArray    array((end != lastBytes, bit8), 50);
        Variable        var("MyVar", array);
        Variable        var2(var);
        std::string     str = "coucou comment ca va?";
        unsigned int    i;
        char            *test = new char[50];
        char            *tmp;
        Register        reg;

        for (i = 0; str[i]; ++i)
            var[i].Set(str[i]);
        var[i].Set(str[i]);

        reg << array;
        std::cout << "test1" << std::endl;
        reg.Dump();

        std::cout << "test2" << std::endl;
        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions

        tmp = test;
        listFunction[array].encode(&test, *var);
        test = tmp;
        if (strcmp(test, str.c_str()) != 0)
            return (false);
        std::cout << test << std::endl;
        listFunction[array].decode(&test, *var2);
        i = 0;
        test = tmp;
        for (i = 0; str[i]; ++i)
        {
            std::cout << var2[i].Get<char>();
            if (var2[i].Get<char>() != test[i])
                return (false);
        }
        std::cout << std::endl;
        delete (test);
        return (true);
    }

    typedef struct  s_test
    {
        char    test1 : 4;
        char    test2 : 4;
        char    test3 : 4;
        char    test4 : 4;
        char    test5 : 4;
        char    test6 : 4;
        char    test7 : 4;
        char    test8 : 4;
        char    test9 : 4;
        char    test10 : 4;
        char    test11 : 4;
        char    test12 : 4;
        char    test13 : 4;
        char    test14 : 4;
        char    test15 : 4;
        char    test16 : 4;
    }               t_test;

    bool test2()
    {
        Bitfield        bit4(4);
        ConstantValue   zero(bit4, 0);
        ConstantValue   one(bit4, 1);
        ConstantValue   two(bit4, 2);
        Variable        lastBytes("LastBytes", bit4, true);
        SwitchCondition sCond = ((lastBytes > two || (lastBytes >= one && lastBytes <= two)), bit4);
        DynamicArray    array(sCond, 50);
        Variable        var("MyVar", array);
        Variable        var2(var);
        unsigned int    i;
        t_test          *test = new t_test[5];
        t_test          *tmp;
        t_test          list = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 1, 0};
        Register        reg;

        for (i = 0; i < 10; ++i)
            var[i].Set((char)4);
        for (; i < 13; ++i)
            var[i].Set((char)3);
        var[i].Set((char) 2);
        var[i + 1].Set((char) 1);
        var[i + 2].Set((char) 0);

        reg << array;
        std::cout << "test1" << std::endl;
        reg.Dump();

        std::cout << "test2" << std::endl;
        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions

        tmp = test;
        listFunction[array].encode((char **)&test, *var);
        test = tmp;
        std::cout << (int)tmp->test1 << (int)tmp->test2 << (int)tmp->test3 << (int)tmp->test4
                  << (int)tmp->test5 << (int)tmp->test6 << (int)tmp->test7 << (int)tmp->test8
                  << (int)tmp->test9 << (int)tmp->test10 << (int)tmp->test11 << (int)tmp->test12
                  << (int)tmp->test13 << (int)tmp->test14 << (int)tmp->test15 << (int)tmp->test16
                  << std::endl;
        if (strncmp((char *)test, (char *)&list, 8) != 0)
            return (false);
        listFunction[array].decode((char **)&test, *var2);
        i = 0;
        test = tmp;
        for (i = 0; i < 16; ++i)
        {
            std::cout << (int)var2[i].Get<char>() << ' ';
            //if (var2[i].Get<char>() != var[i].Get<char>())
                //return (false);
        }
        std::cout << std::endl;
        delete (tmp);
        return (true);
    }

}

using namespace NamespaceTestDynamicArray;

bool testDynamicArray()
{
    return (V(test1(), "test1") && V(test2(), "test2"));
}
