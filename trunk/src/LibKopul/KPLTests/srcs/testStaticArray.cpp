#include "test.h"
#include "StaticArray.h"
#include "Register.h"
#include "Bitfield.h"

using namespace kpl;

namespace NamespaceTestStaticArray
{

    bool    test1()
    {
        //Creation d'un module
        Register        reg;
        Bitfield        bit5(5);
        Bitfield        bit8(8);
        StaticArray     array(bit8, 7);

        // Construction du register
        bit5.SetName("i5");
        array.SetName("arrayi8X7");
        reg << bit5 << array;
        reg.Dump();

        // Compile les fonctions
        FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();
        // Utilisation des fonctions


        // TEST1
        char    *test = new char[10];
        char    *tmp = test;
        char    strToStore[7] = "coucou";
        char    strToLoad[7];
        char    bitToStore = 2;
        char    bitToLoad = 2;

        std::cout << "On veut set un bitfield et un array de valeurs          : " << (int)bitToStore << " " << strToStore << std::endl;

        listFunction[bit5].encode(&test, &bitToStore);
        listFunction[array].encode(&test, &strToStore);
        test = tmp;
        listFunction[bit5].decode(&test, &bitToLoad);
        listFunction[array].decode(&test, &strToLoad);

        std::cout << "Valeurs du bitfield et de l'array read par les fonctions: " << (int)bitToLoad << " " << strToLoad << std::endl;

        if (bitToLoad != bitToStore || strcmp(strToLoad, strToStore) != 0)
            return (false);
        return (true);
    }
}

using namespace NamespaceTestStaticArray;

bool    testStaticArray()
{
    return (V(test1(), "test1"));
}
