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
    Bitfield        bit(8);
    Bitfield        bit4(4);
    StaticArray     staticArray(bit4, 4);

    // Construction du register
    sstruct.SetName("struct");
    bit.SetName("i1");
    staticArray.SetName("array");
    sstruct << bit4 << bit4 << staticArray << bit4 << bit4 << bit4;
    reg << staticArray << sstruct << bit;
    reg.Dump();

    // Compile les fonctions
    FunctionList<int (*)(stream, ...)> &listFunction = *reg.CompileInMemoryMode();

    // Utilisation des fonctions
    char    *test = new char[5];
    char    *tmp = test;
    char    c = 'a';

    test[0] = 'c';
    test[1] = 'z';
    int testouille = (listFunction[bit][ENCODE])(&test, &c);
    std::cout << testouille << std::endl;
    std::cout << "->" << tmp[0] << "<-" << std::endl;
    std::cout << (listFunction[bit][DECODE])(&test, &c) << std::endl;
    std::cout << c << std::endl;
    return (0);
}
