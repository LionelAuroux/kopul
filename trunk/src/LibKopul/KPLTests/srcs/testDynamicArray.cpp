#include "test.h"
#include "Bitfield.h"
#include "Value.h"
#include "Register.h"
#include "DynamicArray.h"

using namespace kpl;

namespace NamespaceTestDynamicArray
{
    bool test1()
    {
        //Creation d'un module
        Register        reg;
        Bitfield        bit8(8);
        Value           endVal(bit8, '\n');
        DynamicArray    array(endVal);

        // Construction du register
        array.SetName("String");
        reg << array;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        char    *test = new char[10];
        char    *tmp = test;
        char    param[] = "coucou\n";
        char    param2[8];

        tmp[9] = 0;
        std::cout << "On veut set dans le buffer   : " << param << std::endl;
        std::cout << "Valeur initiale du buffer    : " << test << std::endl;

        listFunction[array][ENCODE](&test, param);
        std::cout << "Valeur set par la fonction : " << tmp << std::endl;
        if (strncmp(param, tmp, strlen(param)) != 0)
            return (false);


        test = tmp;
        param2[7] = 0;
        listFunction[array].decode(&test, param2);
        std::cout << "Valeur read par la fonctions: " << param2 << std::endl;
        if (strcmp(param, param2) != 0)
            return (false);
        return (true);
    }

    typedef struct  s_test
    {
        int         test1 : 4;
        int         test2 : 4;
        int         test3 : 4;
        int         test4 : 4;
        int         test5 : 4;
        int         test6 : 4;
        int         test7 : 4;
        int         test8 : 4;
    }               t_test;

    std::ostream&   operator << (std::ostream &flux, t_test test)
    {
        flux << test.test1 << " " << test.test2 << " " << test.test3 << " "
                  << test.test4 << " " << test.test5 << " "
                  << test.test6 << " " << test.test7 << " " << test.test8 << " ";
        return (flux);
    }

    bool test2()
    {
        //Creation d'un module
        Register        reg;
        Bitfield        bit4(4);
        Value           endVal(bit4, '\0');
        DynamicArray    array(endVal);

        // Construction du register
        array.SetName("i4_dynamic_array");
        reg << array;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        t_test  *test = new t_test[5];
        t_test  *tmp = test;
        t_test  param[] = {{1, 2, 1, 2, 1, 2, 1, 1}, {1, 2, 1, 2, 0, 2, 1, 1}};
        t_test  param2[2];

        test[1].test6 = 0;
        test[1].test7 = 0;
        test[1].test8 = 0;
        std::cout << "On veut set dans le buffer   : " << param[0] << " " << param[1] << std::endl;
        std::cout << "Valeur initiale du buffer    : " << test[0] << " " << test[1] << std::endl;

        listFunction[array][ENCODE]((char **)&test, param);
        std::cout << "Valeur set par la fonction : " << tmp[0] << " " << tmp[1] << std::endl;
        if (strncmp((char *)&param, (char *)tmp, 5) != 0)
            return (false);


        test = tmp;
        param[1].test6 = 0;
        param[1].test7 = 0;
        param[1].test8 = 0;
        listFunction[array].decode((char **)&test, param2);
        std::cout << "Valeur read par la fonctions: " << param2[0] << " " << param2[1] << std::endl;
        if (strncmp((char *)param, (char *)param2, 5) != 0)
            return (false);
        return (true);
    }

}

using namespace NamespaceTestDynamicArray;

bool testDynamicArray()
{
    return (V(test1(), "test1") && V(test2(), "test2"));
}
