#include "test.h"
#include "StaticStruct.h"
#include "Register.h"
#include "Bitfield.h"

using namespace kpl;

namespace NamespaceTestStaticStruct
{
    
    typedef struct s_test
    {
        int test1 : 5;
        int test2 : 6;
        int test3 : 7;
        int test4 : 12;
    } t_test;

    bool test1()
    {
        //Creation d'un module
        Register        reg;
        Bitfield        bit5(5);
        Bitfield        bit6(6);
        Bitfield        bit7(7);
        Bitfield        bit12(12);
        StaticStruct    myStruct;

        // Construction du register
        bit5.SetName("i5");
        bit6.SetName("i6");
        bit7.SetName("i7");
        bit12.SetName("i14");
        myStruct.SetName("myStruct");
        myStruct << bit5 << bit6 << bit7 << bit12;
        reg << myStruct;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        t_test *test = new t_test[3];
        t_test *tmp = test;
        t_test m;
        t_test m2;

        m.test1 = 5; m.test2 = 6; m.test3 = 7; m.test4 = 12;
        std::cout << "On veut set dans le buffer une structure qui contient: " << m.test1 << " " << m.test2 << " " << m.test3 << " " << m.test4 << std::endl;
        std::cout << "Valeurs initiale de la structure dans le buffer      : " << tmp->test1 << " " << tmp->test2 << " " << tmp->test3 << " " << tmp->test4 << std::endl;


        listFunction[myStruct][ENCODE]((char **) &test, &m);
        std::cout << "Valeurs de la structure set par les fonctions        : " << tmp->test1 << " " << tmp->test2 << " " << tmp->test3 << " " << tmp->test4 << std::endl;
        if (m.test1 != tmp->test1 || m.test2 != tmp->test2 || m.test3 != tmp->test3 || m.test4 != tmp->test4)
            return (false);

        test = tmp;
        listFunction[myStruct].decode((char **) &test, &m2);
        std::cout << "Valeurs de la structure read par les fonctions       : " << m2.test1 << " " << m2.test2 << " " << m2.test3 << " " << m2.test4 << std::endl;
        if (m.test1 != m2.test1 || m.test2 != m2.test2 || m.test3 != m2.test3 || m.test4 != m2.test4)
            return (false);
        return (true);
    }

    typedef struct s_test2
    {
        long long int test1 : 5;
        long long int test2 : 6;
        long long int test3 : 7;
        long long int test4 : 12;

        long long int test5 : 5;
        long long int test6 : 6;
        long long int test7 : 7;
        long long int test8 : 12;
    } t_test2;

    bool test2()
    {
        //Creation d'un module
        Register        reg;
        Bitfield        bit5(5);
        Bitfield        bit6(6);
        Bitfield        bit7(7);
        Bitfield        bit12(12);
        StaticStruct    myStruct;

        // Construction du register
        bit5.SetName("i5");
        bit6.SetName("i6");
        bit7.SetName("i7");
        bit12.SetName("i14");
        myStruct.SetName("myStruct");
        myStruct << bit5 << bit6 << bit7 << bit12;
        reg << myStruct;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        t_test2 *test = new t_test2[3];
        t_test2 *tmp = test;
        t_test  m1;
        t_test  m2;
        t_test  m3;
        t_test  m4;

        m1.test1 = 5; m1.test2 = 6; m1.test3 = 7; m1.test4 = 12;
        m2.test1 = 4; m2.test2 = 5; m2.test3 = 6; m2.test4 = 11;
        std::cout << "On veut set dans le buffer deux structures qui contiennent: " << std::endl
                  << m1.test1 << " " << m1.test2 << " " << m1.test3 << " " << m1.test4 << std::endl
                  << m2.test1 << " " << m2.test2 << " " << m2.test3 << " " << m2.test4 << std::endl;
        std::cout << "Valeurs initiale des structures dans le buffer            : " << std::endl
                  << tmp->test1 << " " << tmp->test2 << " " << tmp->test3 << " " << tmp->test4 << std::endl
                  << tmp->test5 << " " << tmp->test6 << " " << tmp->test7 << " " << tmp->test8 << std::endl;


        listFunction[myStruct][ENCODE]((char **) &test, &m1);
        listFunction[myStruct][ENCODE]((char **) &test, &m2);
        std::cout << "Valeurs des structures set par les fonctions              : " << std::endl
                  << tmp->test1 << " " << tmp->test2 << " " << tmp->test3 << " " << tmp->test4 << std::endl
                  << tmp->test5 << " " << tmp->test6 << " " << tmp->test7 << " " << tmp->test8 << std::endl;
        if (m1.test1 != tmp->test1 || m1.test2 != tmp->test2 || m1.test3 != tmp->test3 || m1.test4 != tmp->test4 ||
            m2.test1 != tmp->test5 || m2.test2 != tmp->test6 || m2.test3 != tmp->test7 || m2.test4 != tmp->test8)
            return (false);

        test = tmp;
        listFunction[myStruct].decode((char **) &test, &m3);
        listFunction[myStruct].decode((char **) &test, &m4);
        std::cout << "Valeurs des structures read par les fonctions             : " << std::endl
                  << m3.test1 << " " << m3.test2 << " " << m3.test3 << " " << m3.test4 << std::endl
                  << m4.test1 << " " << m4.test2 << " " << m4.test3 << " " << m4.test4 << std::endl;
        if (m1.test1 != m3.test1 || m1.test2 != m3.test2 || m1.test3 != m3.test3 || m1.test4 != m3.test4 ||
            m2.test1 != m4.test1 || m2.test2 != m4.test2 || m2.test3 != m4.test3 || m2.test4 != m4.test4)
            return (false);
        return (true);
    }

}

using namespace NamespaceTestStaticStruct;

bool testStaticStruct()
{
    return (V(test1(), "test1") && V(test2(), "test2"));
}
