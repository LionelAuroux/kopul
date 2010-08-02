#include "StaticStruct.h"

using namespace kpl;

int			StaticStruct::buildEncodeFunction(llvm::Function *)
{
	std::cout << "build function encode de STRUCT" << std::endl;
	return (0);
}

int			StaticStruct::buildDecodeFunction(llvm::Function *)
{
	std::cout << "build function decode de STRUCT" << std::endl;
	return (0);
}
