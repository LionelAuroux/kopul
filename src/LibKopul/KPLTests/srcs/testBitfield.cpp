#include "test.h"
#include "Register.h"
#include "Bitfield.h"

using namespace kpl;

namespace NamespaceTestBitfield
{

    typedef struct s_test
    {
        int test1 : 5;
        int test2 : 6;
        int test3 : 7;
        int test4 : 14;
    } t_test;

    bool test1()
    {
        //Creation d'un module
        Register reg;
        Bitfield bit5(5);
        Bitfield bit6(6);
        Bitfield bit7(7);
        Bitfield bit14(14);

        // Construction du register
        bit5.SetName("i5");
        bit6.SetName("i6");
        bit7.SetName("i7");
        bit14.SetName("i14");
        reg << bit5 << bit6 << bit7 << bit14;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        t_test *test = new t_test[3];
        t_test *tmp = test;
        int c1, c2, c3, c4;
        t_test m;

        c1 = m.test1 = 5;
        c2 = m.test2 = 6;
        c3 = m.test3 = 7;
        c4 = m.test4 = 14;
        std::cout << "On veut set dans le buffer   : " << m.test1 << " " << m.test2 << " " << m.test3 << " " << m.test4 << std::endl;
        std::cout << "Valeur initiale du buffer    : " << tmp->test1 << " " << tmp->test2 << " " << tmp->test3 << " " << tmp->test4 << std::endl;

        listFunction[bit5][ENCODE]((char **) & test, &c1);
        listFunction[bit6].encode((char **) & test, &c2);
        listFunction[bit7].encode((char **) & test, &c3);
        listFunction[bit14].encode((char **) & test, &c4);
        std::cout << "Valeur set par les fonctions : " << tmp->test1 << " " << tmp->test2 << " " << tmp->test3 << " " << tmp->test4 << std::endl;
        if (m.test1 != tmp->test1 || m.test2 != tmp->test2 || m.test3 != tmp->test3 || m.test4 != tmp->test4)
            return (false);


        test = tmp;
        c1 = c2 = c3 = c4 = 0;
        listFunction[bit5].decode((char **) & test, &c1);
        listFunction[bit6].decode((char **) & test, &c2);
        listFunction[bit7].decode((char **) & test, &c3);
        listFunction[bit14].decode((char **) & test, &c4);
        std::cout << "Valeur read par les fonctions: " << c1 << " " << c2 << " " << c3 << " " << c4 << std::endl;
        if (m.test1 != c1 || m.test2 != c2 || m.test3 != c3 || m.test4 != c4)
            return (false);
        return (true);
    }

}

using namespace NamespaceTestBitfield;

bool testBitfield()
{
    return (V(test1(), "test1"));
}
