#include <exception>
#include <string>

#include "test.h"
#include "Bitfield.h"
#include "DynamicArray.h"
#include "Variable.h"
#include "VariableIterator.h"
#include "ConstantValue.h"

using namespace kpl;

namespace NamespaceTestVariable
{
    bool test1()
    {
        Bitfield    bit8(8);
        Variable    var("MyVar", bit8);

        std::cout << "Coucou" << std::endl;
        *(char *)(*var) = 'b';
        if (var.Get<char>() != 'b')
            return (false);
        var.Set('a');
        if (var.Get<char>() != 'a')
            return (false);
        try
        {
            var[1];
        }
        catch (std::exception)
        {
            return (true);
        }
        return (false);
    }

    bool test2()
    {
        Bitfield        bit8(8);
        ConstantValue   end(bit8, '\0');
        Variable        lastBytes("LastBytes", bit8, true);
        DynamicArray    array((lastBytes != end, bit8), 50);
        Variable        var("MyVar", array);
        std::string     test = "coucou comment ca va?";
        unsigned int    i;

        for (i = 0; test[i]; ++i)
            var[i].Set(test[i]);
        var[i].Set(test[i]);
        for (i = 0; test[i]; ++i)
        {
            std::cout << var[i].Get<char>();
            if (var[i].Get<char>() != test[i])
                return (false);
        }
        std::cout << std::endl;
        return (true);
    }

}

using namespace NamespaceTestVariable;

bool testVariable()
{
    return (V(test1(), "test1") && V(test2(), "test2"));
}

