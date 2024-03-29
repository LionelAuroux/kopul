# include "test.h"

bool    V(bool isOk, const std::string& str)
{
    if (isOk)
        std::cout << str << " succeeded" << std::endl;
    else
        std::cout << str << " failed" << std::endl;
    return (isOk);
}

int     main()
{
    if (V(testBitfield(), "testBitfield") && V(testStaticStruct(), "testStaticStruct")
        && V(testStaticArray(), "testStaticArray") && V(testConstantValue(), "testConstantValue")
        && V(testVariable(), "testVariable") && V(testSwitch(), "testSwitch")
        && V(testDynamicArray(), "testDynamicArray"))
        std::cout << "All the tests succeeded" << std::endl;
    else
        std::cout << "Some Errors occured" << std::endl;
    return (0);
}